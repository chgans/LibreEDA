#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QIcon>
#include <QMap>

class ISettingsPage;
class QLineEdit;
class QLabel;
class QStackedLayout;
class QListWidget;
class QTabWidget;
class QListWidgetItem;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
    void currentCategoryChanged(QListWidgetItem *current,
                                QListWidgetItem *previous);
    void currentPageChanged();

private:
    QLineEdit *m_filterLineEdit;
    QLabel *m_headerLabel;
    QListWidget *m_categoryListWidget;
    QStackedLayout *m_stackedLayout;
    void createGui();

    QList<ISettingsPage *> m_pages;
    QList<ISettingsPage *> m_visitedPages;
    ISettingsPage *m_currentPage;
    QMap<QWidget *, ISettingsPage *> m_widgetToPage;

    QStringList m_categoryNames;
    QMap<QString, QIcon> m_categoryIcons;
    QMap<QString, QTabWidget *> m_categoryTabWidgets;
    QMap<QListWidgetItem *, QTabWidget *> m_listItemToTabWidget;
    void populateSettingsPages();

    // QDialog interface
public slots:
    void done(int code);
    void accept();
    void reject();
    void apply();
};



#endif // SETTINGSDIALOG_H
