#include "navigationdockwidget.h"
#include "inavigationviewfactory.h"

#include <QComboBox>
#include <QToolBar>
#include <QSettings>
#include <QVariant>
#include <QHBoxLayout>

NavigationDockWidget::NavigationDockWidget(QWidget *parent):
    QDockWidget(parent),
    m_comboBox(new QComboBox),
    m_toolBar(new QToolBar)
{
    setTitleBarWidget(m_toolBar);
    m_comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

NavigationDockWidget::~NavigationDockWidget()
{

}

void NavigationDockWidget::setFactories(const QList<INavigationViewFactory *> &factories)
{
    Q_ASSERT(m_factories.isEmpty());
    Q_ASSERT(m_comboBox->count() == 0);
    m_factories = factories;
    foreach (INavigationViewFactory *factory, m_factories) {
        m_comboBox->addItem(factory->displayName(),
                            QVariant::fromValue<NavigationView *>(factory->createView()));
    }
    int defaultIndex = 0;
    m_comboBox->setCurrentIndex(defaultIndex);
    activateNavigationView(defaultIndex);
    void (QComboBox::*comboSignal)(int) = &QComboBox::currentIndexChanged;
    connect(m_comboBox, comboSignal,
            this, &NavigationDockWidget::activateNavigationView);
}

void NavigationDockWidget::saveSettings(QSettings *settings)
{

}

void NavigationDockWidget::restoreSettings(QSettings *settings)
{

}

void NavigationDockWidget::activateNavigationView(int index)
{
    NavigationView *view = m_comboBox->itemData(index).value<NavigationView *>();
    m_toolBar->clear();
    m_toolBar->addWidget(m_comboBox);
    foreach (QWidget *widget, view->toolBarWidgets) {
        m_toolBar->addWidget(widget);
    }
    setWidget(view->widget);
    view->widget->show();
}

