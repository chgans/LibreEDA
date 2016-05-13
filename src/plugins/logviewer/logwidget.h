#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include "logviewer_global.h"

#include <QFrame>

namespace Ui {
class LogWidget;
}

class LogModel;
class LogSortFilterProxyModel;
class QMenu;

class LogWidget : public QFrame
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget *parent = nullptr);
    ~LogWidget();

    LogModel *model();
    void setModel(LogModel *model);

    bool canNavigate() const;
    bool canGoToNext() const;
    bool canGoToPrevious() const;

protected slots:
    void updateNavigationState();
    void goToNext();
    void goToPrevious();
    void updateCategoryList(const QStringList &categories);
    void filterCategoriesTriggered(QAction *action);

protected:
    void setCategoryVisibility(const QString &categoryName, bool visible);

private:
    Ui::LogWidget *ui;
    LogModel *m_model;
    LogSortFilterProxyModel *m_filterModel;
    QMenu *m_categoriesMenu;
};

#endif // LOGWIDGET_H
