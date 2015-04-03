#include "taskeditwidget.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

TaskEditWidget::TaskEditWidget(QWidget *parent) :
    QGroupBox(parent),
    m_centralWidget(nullptr)
{
    setTitle("Operation");
    setStyleSheet("QGroupBox {\n"
                  "    border: 1px solid gray;"
                  "    border-radius: 5px;"
                  "    margin-top: 0.5em;"
                  "}\n"
                  "QGroupBox::title {\n"
                  "    subcontrol-origin: margin;"
                  "    left: 10px;"
                  "    padding: 0 3px 0 3px;"
                  "}\n");
    m_layout = new QVBoxLayout();
    setLayout(m_layout);

    m_centralPlaceHolderWidget = new QWidget();
    m_centralWidget = m_centralPlaceHolderWidget;
    m_layout->addWidget(m_centralPlaceHolderWidget);

    m_cancelButton = new QPushButton();
    m_cancelButton->setIcon(QIcon::fromTheme("dialog-error"));
    m_cancelButton->setText("Cancel");
    // TODO: connect cancel button
    m_layout->addWidget(m_cancelButton);

    m_layout->addStretch();
}

void TaskEditWidget::setCentralWidget(QWidget *widget)
{
    QWidget *newWidget = widget != nullptr ? widget : m_centralPlaceHolderWidget;

    m_layout->removeWidget(m_centralWidget);
    m_centralWidget->hide();

    m_centralWidget = newWidget;
    m_centralWidget->show();
    m_layout->insertWidget(0, m_centralWidget);

    if (m_centralWidget->layout() != nullptr) {
        m_centralWidget->layout()->setMargin(0);
    }
}

bool TaskEditWidget::focusNextPrevChild(bool next)
{
    qDebug() << __PRETTY_FUNCTION__;
    return QWidget::focusNextPrevChild(next);
}

void TaskEditWidget::focusInEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QWidget::focusInEvent(event);
}

void TaskEditWidget::focusOutEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QWidget::focusOutEvent(event);}
