#include "settingsdialog.h"
#include "isettingspage.h"
#include "extension/pluginmanager.h"

#include <QGridLayout>
#include <QDialogButtonBox>
#include <QPushButton>

#include <QLineEdit>
#include <QLabel>
#include <QStackedLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTabWidget>

SettingsDialog::SettingsDialog(QWidget *parent):
    QDialog(parent)
{
    createGui();
    populateSettingsPages();
    m_categoryListWidget->setFocus();
    connect(m_categoryListWidget, &QListWidget::currentItemChanged,
            this, &SettingsDialog::currentCategoryChanged);
}

void SettingsDialog::currentCategoryChanged(QListWidgetItem *current,
                                            QListWidgetItem */*previous*/)
{
    QTabWidget *tabWidget = m_listItemToTabWidget.value(current);
    m_stackedLayout->setCurrentWidget(tabWidget);
}

void SettingsDialog::currentPageChanged()
{

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
    for (auto page: PluginManager::getObjects<ISettingsPage>())
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
        return;

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

void SettingsDialog::done(int code)
{
    // TODO: save our own settings: current page and dialog size

    QDialog::done(code);
}

void SettingsDialog::accept()
{
    foreach (ISettingsPage *page, m_visitedPages)
    {
        page->apply();
    }
    foreach (ISettingsPage *page, m_pages)
    {
        page->finish();
    }
    done(QDialog::Accepted);
}

void SettingsDialog::reject()
{
    foreach (ISettingsPage *page, m_pages)
    {
        page->finish();
    }
    done(QDialog::Rejected);
}

void SettingsDialog::apply()
{
    foreach (ISettingsPage *page, m_visitedPages)
    {
        page->apply();
    }
}
