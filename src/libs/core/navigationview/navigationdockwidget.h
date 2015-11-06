#ifndef NAVIGATIONDOCKWIDGET_H
#define NAVIGATIONDOCKWIDGET_H

#include "core/core_global.h"

#include <QDockWidget>
#include <QList>
#include <QMap>

class INavigationViewFactory;
class QSettings;

class QComboBox;
class QToolBar;

class CORE_EXPORT NavigationDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    NavigationDockWidget(QWidget *parent = nullptr);
    ~NavigationDockWidget();

    void setFactories(const QList<INavigationViewFactory*> &factories);

    void saveSettings(QSettings *settings);
    void restoreSettings(QSettings *settings);

private slots:
    void activateNavigationView(int index);

private:
    QList<INavigationViewFactory*> m_factories;
    QComboBox *m_comboBox;
    QToolBar *m_toolBar;
};

#endif // NAVIGATIONDOCKWIDGET_H
