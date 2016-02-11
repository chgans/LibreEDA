#include "outputdockwidget.h"
#include "ioutputpanefactory.h"

#include <QTabWidget>

OutputDockWidget::OutputDockWidget(QWidget *parent):
    QDockWidget(parent)
{
    m_tabWidget = new QTabWidget;
    m_tabWidget->setTabPosition(QTabWidget::South);
    setWidget(m_tabWidget);
}

OutputDockWidget::~OutputDockWidget()
{

}

void OutputDockWidget::setFactories(const QList<IOutputPaneFactory *> &factories)
{
    foreach (IOutputPaneFactory *factory, factories) {
        m_tabWidget->insertTab(factory->priority(),
                               factory->createWidget(),
                               factory->displayName());
    }
}
