#include "widget/taskoptionwidget.h"

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
    m_layout = new QVBoxLayout();
    setLayout(m_layout);

    m_layout->addStretch();
}

void TaskOptionWidget::setCentralWidget(QWidget *widget)
{
    if (m_centralWidget != nullptr) {
        m_layout->removeWidget(m_centralWidget);
        m_centralWidget->hide();
    }

    if (widget != nullptr) {
        m_layout->insertWidget(0, widget);
        // Align central widget and our extra ones (cancel button, ...)
        if (widget->layout() != nullptr) {
            widget->layout()->setMargin(0);
        }
        widget->show();
    }

    m_centralWidget = widget;
}


void TaskOptionWidget::focusInEvent(QFocusEvent *event)
{
    qDebug() << __FUNCTION__;
    QGroupBox::focusInEvent(event);
}

void TaskOptionWidget::focusOutEvent(QFocusEvent *event)
{
    qDebug() << __FUNCTION__;
    QGroupBox::focusOutEvent(event);
}

bool TaskOptionWidget::focusNextPrevChild(bool next)
{
    qDebug() << __FUNCTION__;
    return QGroupBox::focusNextPrevChild(next);
}
