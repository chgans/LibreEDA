#include "navigationdockwidget.h"
#include "inavigationviewfactory.h"

#include <QComboBox>
#include <QToolBar>
#include <QSettings>
#include <QVariant>
#include <QHBoxLayout>
#include <QAction>

// TODO: needs a place holder in case there's no view to choose from
//
NavigationDockWidget::NavigationDockWidget(QWidget *parent):
    QDockWidget(parent),
    m_comboBox(new QComboBox),
    m_toolBar(new QToolBar)
{
    setWindowTitle("Navigation");
    setTitleBarWidget(m_toolBar);
    m_comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    void (QComboBox::*comboSignal)(int) = &QComboBox::currentIndexChanged;
    connect(m_comboBox, comboSignal,
            this, &NavigationDockWidget::activateNavigationView);
}

NavigationDockWidget::~NavigationDockWidget()
{
    m_comboBox->disconnect(this);
    while (m_comboBox->count()) {
        NavigationView *view = m_comboBox->itemData(0).value<NavigationView *>();
        m_comboBox->removeItem(0);
        delete view;
    }
}

void NavigationDockWidget::setFactories(const QList<INavigationViewFactory *> &factories)
{
    Q_ASSERT(m_factories.isEmpty());
    Q_ASSERT(m_comboBox->count() == 0);
    m_factories = factories;

    // loadFactories()
    for (INavigationViewFactory *factory: m_factories) {
        m_comboBox->addItem(factory->displayName(),
                            QVariant::fromValue<NavigationView *>(factory->createView()));
    }
    if (m_comboBox->count()) {
        int defaultIndex = 0;
        m_comboBox->setCurrentIndex(defaultIndex);
        activateNavigationView(defaultIndex);
    }

}

void NavigationDockWidget::saveSettings(QSettings *settings)
{
    Q_UNUSED(settings);
}

void NavigationDockWidget::restoreSettings(QSettings *settings)
{
    Q_UNUSED(settings);
}

void NavigationDockWidget::activateNavigationView(int index)
{
    NavigationView *view = m_comboBox->itemData(index).value<NavigationView *>();
    m_toolBar->clear();
    QAction *action = m_toolBar->addWidget(m_comboBox);
    action->setVisible(true);
    for (QWidget *widget: view->toolBarWidgets) {
        m_toolBar->addWidget(widget);
    }
    setWidget(view->widget);
}

