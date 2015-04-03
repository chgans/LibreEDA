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
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);

    m_centralPlaceHolderWidget = new QWidget();
    m_centralWidget = m_centralPlaceHolderWidget;
    layout->addWidget(m_centralPlaceHolderWidget);

    m_cancelButton = new QPushButton();
    m_cancelButton->setIcon(QIcon::fromTheme("dialog-error"));
    m_cancelButton->setText("Cancel");
    // TODO: connect cancel button
    layout->addWidget(m_cancelButton);

    layout->addStretch();
}

void TaskEditWidget::setCentralWidget(QWidget *widget)
{
    QWidget *newWidget = widget != nullptr ? widget : m_centralPlaceHolderWidget;
    layout()->replaceWidget(m_centralWidget,
                            newWidget);

    if (m_centralWidget != nullptr)
        m_centralWidget->setParent(nullptr);

    m_centralWidget = newWidget;

    if (m_centralWidget->layout() != nullptr)
        m_centralWidget->layout()->setMargin(0);
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
