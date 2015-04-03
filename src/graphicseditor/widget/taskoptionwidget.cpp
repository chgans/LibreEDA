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
    m_layout = new QVBoxLayout();
    setLayout(m_layout);

    m_centralPlaceHolderWidget = new QWidget();
    m_centralWidget = m_centralPlaceHolderWidget;
    m_layout->addWidget(m_centralWidget);

    m_layout->addStretch();
}

void TaskOptionWidget::setCentralWidget(QWidget *widget)
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


void TaskOptionWidget::focusInEvent(QFocusEvent *)
{
    qDebug() << __PRETTY_FUNCTION__;
}

void TaskOptionWidget::focusOutEvent(QFocusEvent *)
{
    qDebug() << __PRETTY_FUNCTION__;
}
