#include "settingsdialog.h"
#include "isettingspage.h"
#include "extension/pluginmanager.h"
#include "core.h"

#include <QGridLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include <QLineEdit>
#include <QLabel>
#include <QStackedLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTabWidget>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent):
    QDialog(parent)
{
    createGui();
    populateSettingsPages();
    restoreState();

    m_categoryListWidget->setFocus();
    connect(m_categoryListWidget, &QListWidget::currentItemChanged,
            this, &SettingsDialog::currentCategoryChanged);
}

void SettingsDialog::currentCategoryChanged(QListWidgetItem *current,
                                            QListWidgetItem *previous)
{
    if (previous != nullptr)
    {
        QTabWidget *previousTabWidget = m_listItemToTabWidget.value(previous);
        previousTabWidget->disconnect(this);
    }
    QTabWidget *currentTabWidget = m_listItemToTabWidget.value(current);
    connect(currentTabWidget, &QTabWidget::currentChanged,
            this, &SettingsDialog::currentPageChanged);
    m_stackedLayout->setCurrentWidget(currentTabWidget);
    currentPageChanged(currentTabWidget->currentIndex());
}

void SettingsDialog::currentPageChanged(int /*currentTabindex*/)
{
    QTabWidget *currentTabWidget = m_listItemToTabWidget.value(m_categoryListWidget->currentItem());
    m_currentPage = m_widgetToPage.value(currentTabWidget->currentWidget());
    if (!m_visitedPages.contains(m_currentPage))
    {
        m_visitedPages.append(m_currentPage);
    }
}

void SettingsDialog::createGui()
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                       QDialogButtonBox::Apply |
                                                       QDialogButtonBox::Cancel);
    connect(buttonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked,
            this, &SettingsDialog::apply);
    connect(buttonBox, &QDialogButtonBox::accepted,
            this, &SettingsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected,
            this, &SettingsDialog::reject);

    m_headerLabel = new QLabel;
    QFont headerFont = m_headerLabel->font();
    headerFont.setBold(true);
    m_headerLabel->setFont(headerFont);
    m_headerLabel->setEnabled(false);

    m_filterLineEdit = new QLineEdit;
    m_filterLineEdit->setPlaceholderText("Filter...");
    m_filterLineEdit->setEnabled(false);

    m_categoryListWidget = new QListWidget;
    m_categoryListWidget->setViewMode(QListView::ListMode);
    m_categoryListWidget->setWrapping(false);
    m_categoryListWidget->setUniformItemSizes(true);
    m_categoryListWidget->setIconSize(QSize(48, 48));

    m_stackedLayout = new QStackedLayout;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(m_filterLineEdit,   0, 0, 1, 1);
    layout->addWidget(m_headerLabel,      0, 1, 1, 1);
    layout->addWidget(m_categoryListWidget, 1, 0, 1, 1);
    layout->addLayout(m_stackedLayout,    1, 1, 1, 1);
    layout->addWidget(buttonBox,          2, 0, 1, 2);
    layout->setColumnStretch(1, 4);
    setLayout(layout);
}

void SettingsDialog::populateSettingsPages()
{
    for (auto page : PluginManager::getObjects<ISettingsPage>())
    {
        const QString &categoryName = page->categoryName();
        if (!m_categoryNames.contains(categoryName))
        {
            m_categoryNames.append(categoryName);
            m_categoryIcons.insert(categoryName,
                                   page->categoryIcon());
            auto tabWidget = new QTabWidget;
            m_categoryTabWidgets.insert(categoryName,
                                        tabWidget);
            m_stackedLayout->addWidget(tabWidget);
            auto listItem = new QListWidgetItem(page->categoryIcon(),
                                                page->categoryName());
            m_categoryListWidget->addItem(listItem);
            m_listItemToTabWidget.insert(listItem, tabWidget);
        }

        auto tabWidget = m_categoryTabWidgets.value(categoryName);
        auto widget = page->widget();
        tabWidget->addTab(widget, page->pageName());
        m_widgetToPage.insert(widget, page);
        m_pages.append(page);
    }

    m_currentPage = nullptr;
    if (m_categoryNames.isEmpty())
    {
        return;
    }

    m_categoryListWidget->setCurrentRow(0);
    auto listItem = m_categoryListWidget->item(0);
    m_headerLabel->setText(listItem->text());
    auto tabWidget = m_listItemToTabWidget.value(listItem);
    m_stackedLayout->setCurrentWidget(tabWidget);
    if (tabWidget->count() != 0)
    {
        tabWidget->setCurrentIndex(0);
        m_currentPage = m_widgetToPage.value(tabWidget->currentWidget());
        m_visitedPages.append(m_currentPage);
    }
}

void SettingsDialog::saveState()
{
    QSettings *settings = Core::settings();
    settings->beginGroup("core.settingsDialog");
    settings->setValue("geometry", geometry());
    settings->setValue("category", m_currentPage->categoryName());
    settings->setValue("page", m_currentPage->pageName());
    settings->endGroup();
    settings->sync();
}

void SettingsDialog::restoreState()
{
    QSettings *settings = Core::settings();
    settings->beginGroup("core.settingsDialog");
    setGeometry(settings->value("geometry").toRect());

    QString category = settings->value("category").toString();
    if (!m_categoryNames.contains(category))
    {
        settings->endGroup();
        return;
    }
    m_categoryListWidget->setCurrentRow(m_categoryNames.indexOf(category));
    QTabWidget *tabWidget = m_categoryTabWidgets.value(category);
    m_stackedLayout->setCurrentWidget(tabWidget);

    QString page = settings->value("page").toString();
    for (int i = 0; i < tabWidget->count(); i++)
    {
        auto widget = tabWidget->widget(i);
        auto ipage = m_widgetToPage.value(widget);
        if (ipage->pageName() == page)
        {
            m_currentPage = ipage;
            m_visitedPages.append(m_currentPage);
            tabWidget->setCurrentWidget(widget);
            break;
        }
    }
    settings->endGroup();
}

void SettingsDialog::done(int code)
{
    saveState();
    QDialog::done(code);
}

void SettingsDialog::accept()
{
    for (ISettingsPage *page : m_visitedPages)
    {
        page->apply();
    }
    for (ISettingsPage *page : m_pages)
    {
        page->finish();
    }
    done(QDialog::Accepted);
}

void SettingsDialog::reject()
{
    for (ISettingsPage *page : m_pages)
    {
        page->finish();
    }
    done(QDialog::Rejected);
}

void SettingsDialog::apply()
{
    for (ISettingsPage *page : m_visitedPages)
    {
        page->apply();
    }
}
