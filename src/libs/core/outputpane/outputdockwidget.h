#ifndef OUTPUTDOCKWIDGET_H
#define OUTPUTDOCKWIDGET_H

#include "core/core_global.h"

#include <QDockWidget>
#include <QMap>

class IOutputPaneFactory;
class QTabWidget;
class QToolBar;

class CORE_EXPORT OutputDockWidget : public QDockWidget
{
public:
    explicit OutputDockWidget(QWidget *parent = nullptr);
    ~OutputDockWidget();

    void setFactories(const QList<IOutputPaneFactory*> &factories);

private:
    QTabWidget *m_tabWidget;
};

#endif // OUTPUTDOCKWIDGET_H
