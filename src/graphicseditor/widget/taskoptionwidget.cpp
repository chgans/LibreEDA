#include "taskoptionwidget.h"

#include <QDebug>
#include <QVBoxLayout>

TaskOptionWidget::TaskOptionWidget(QWidget *parent) :
    QGroupBox(parent),
    m_centralWidget(nullptr)
{
    setTitle("Options");
    setAlignment(Qt::AlignCenter);
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

    layout->addStretch();
}

void TaskOptionWidget::setCentralWidget(QWidget *widget)
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


void TaskOptionWidget::focusInEvent(QFocusEvent *)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void TaskOptionWidget::focusOutEvent(QFocusEvent *)
{
    qDebug() << __PRETTY_FUNCTION__;
}
