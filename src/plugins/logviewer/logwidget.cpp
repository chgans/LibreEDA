#include "logwidget.h"
#include "logmodel.h"
#include "ui_logwidget.h"
#include "logsortfilterproxymodel.h"

#include <QMenu>

LogWidget::LogWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LogWidget),
    m_model(nullptr),
    m_filterModel(new LogSortFilterProxyModel(this)),
    m_categoriesMenu(new QMenu(this))
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    ui->configToolButton->setMenu(m_categoriesMenu);
    connect(m_categoriesMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(filterCategoriesTriggered(QAction*)));

    connect(ui->infoToolButton, &QToolButton::toggled,
            m_filterModel, &LogSortFilterProxyModel::setFilterIncludesDebugMessages);
    connect(ui->warningToolButton, &QToolButton::toggled,
            m_filterModel, &LogSortFilterProxyModel::setFilterIncludesWarningMessages);
    connect(ui->errorToolButton, &QToolButton::toggled,
            m_filterModel, &LogSortFilterProxyModel::setFilterIncludesCriticalMessages);
    connect(ui->errorToolButton, &QToolButton::toggled,
            m_filterModel, &LogSortFilterProxyModel::setFilterIncludesFatalMessages);
    ui->infoToolButton->setChecked(true);
    ui->warningToolButton->setChecked(true);
    ui->errorToolButton->setChecked(true);

    connect(ui->previousToolButton, &QToolButton::clicked,
            this, &LogWidget::goToPrevious);
    connect(ui->nextToolButton, &QToolButton::clicked,
            this, &LogWidget::goToNext);

    connect(m_filterModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(updateNavigationState()));
    connect(m_filterModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(updateNavigationState()));
    connect(m_filterModel, SIGNAL(modelReset()),
            this, SLOT(updateNavigationState()));
    updateNavigationState();

    m_filterModel->setFilterKeyColumn(-1); // filter on all columns
    connect(ui->filterLineEdit, SIGNAL(textChanged(QString)),
            m_filterModel, SLOT(setFilterFixedString(QString)));
}

LogWidget::~LogWidget()
{
    delete ui;
}

LogModel *LogWidget::model()
{
    return m_model;
}

void LogWidget::setModel(LogModel *model)
{
    if (m_model) {
        m_model->disconnect(this);
        ui->clearToolButton->disconnect(m_model);
        ui->tableView->selectionModel()->disconnect(this);
    }

    m_model = model;
    m_filterModel->setSourceModel(model);
    ui->tableView->setModel(m_filterModel);

    if (m_model) {
#if 0
        connect(m_model, &LoggingModel::rowsInserted,
                this, [this]() {
            this->ui->tableView->scrollToBottom();
        });
#endif
        connect(ui->clearToolButton, &QToolButton::clicked,
                this, [this](bool checked) {
            Q_UNUSED(checked);
            this->m_model->clearMessages();
        });
        connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this, SLOT(updateNavigationState()));
        connect(m_model, SIGNAL(categoryListChanged(QStringList)),
                this, SLOT(updateCategoryList(QStringList)));
    }
}

bool LogWidget::canNavigate() const
{
    return m_filterModel->rowCount() != 0;
}

bool LogWidget::canGoToNext() const
{
    if (!canNavigate())
        return false;

    if (!ui->tableView->currentIndex().isValid())
        return true;

    return ui->tableView->currentIndex().row() != m_filterModel->rowCount() - 1;
}

bool LogWidget::canGoToPrevious() const
{
    if (!canNavigate())
        return false;

    if (!ui->tableView->currentIndex().isValid())
        return true;

    return ui->tableView->currentIndex().row() != 0;
}

void LogWidget::goToNext()
{
    if (!canNavigate())
        return;

    QModelIndex current = ui->tableView->currentIndex();
    if (!current.isValid()) {
        ui->tableView->setCurrentIndex(m_filterModel->index(0, 0));
        return;
    }

    QModelIndex next = m_filterModel->index(current.row()+1, 0);
    ui->tableView->setCurrentIndex(next);

}

void LogWidget::goToPrevious()
{
    if (!canNavigate())
        return;

    QModelIndex current = ui->tableView->currentIndex();
    if (!current.isValid()) {
        ui->tableView->setCurrentIndex(m_filterModel->index(0, 0));
        return;
    }

    QModelIndex next = m_filterModel->index(current.row()-1, 0);
    ui->tableView->setCurrentIndex(next);
}

void LogWidget::updateCategoryList(const QStringList &categories)
{
    m_categoriesMenu->clear();
    foreach (QString category, categories) {
        QAction *action = new QAction(m_categoriesMenu);
        action->setCheckable(true);
        action->setText(category);
        action->setData(category);
        action->setChecked(m_filterModel->filterIncludesCategoryName(category));
        m_categoriesMenu->addAction(action);
    }
}

void LogWidget::filterCategoriesTriggered(QAction *action)
{
    QString category = action->data().toString();
    setCategoryVisibility(category, action->isChecked());
}

void LogWidget::setCategoryVisibility(const QString &categoryName, bool visible)
{
    m_filterModel->setFilterIncludesCategoryName(categoryName, visible);
}

void LogWidget::updateNavigationState()
{
    ui->nextToolButton->setEnabled(canGoToNext());
    ui->previousToolButton->setEnabled(canGoToPrevious());
}
