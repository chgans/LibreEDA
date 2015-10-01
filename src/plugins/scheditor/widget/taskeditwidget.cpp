#include "widget/taskeditwidget.h"

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

    m_cancelButton = new QPushButton();
    m_cancelButton->setShortcut(QKeySequence(Qt::Key_Escape));
    m_cancelButton->setIcon(QIcon::fromTheme("dialog-error"));
    m_cancelButton->setText("Cancel");
    // TODO: connect cancel button

    m_layout->addStretch();
}

// Layout rearragement is done in such a way that keeps tab order
void TaskEditWidget::setCentralWidget(QWidget *widget)
{
    if (m_centralWidget != nullptr) {
        m_layout->removeWidget(m_centralWidget);
        m_centralWidget->hide();
        m_layout->removeWidget(m_cancelButton);
    }

    if (widget != nullptr) {
        m_layout->insertWidget(0, widget);
        m_layout->insertWidget(1, m_cancelButton);
        // Align central widget and our extra ones (cancel button, ...)
        if (widget->layout() != nullptr) {
            widget->layout()->setMargin(0);
        }
        widget->show();
    }
    else {
        m_layout->insertWidget(0, m_cancelButton);
    }

    m_centralWidget = widget;

    // m_centralWidget will receive the focus instead of us
    setFocusProxy(m_centralWidget);
}

QWidget *TaskEditWidget::findLastChildToFocus(QObject *parent)
{
    qDebug() << __PRETTY_FUNCTION__ << "parent" << parent;
    for (int i = parent->children().count() - 1; i >= 0; i--) {
        QObject *childObject = parent->children().at(i);
        qDebug() << __PRETTY_FUNCTION__ << "child object" << childObject;
        QWidget *childWidget = qobject_cast<QWidget*>(childObject);
        if (childWidget == nullptr)
            continue;
        qDebug() << __PRETTY_FUNCTION__ << "child widget" << childWidget;
        if (childWidget->focusPolicy() != Qt::NoFocus) {
            qDebug() << __PRETTY_FUNCTION__ << "Found!";
            return childWidget;
        }
        else
            return findLastChildToFocus(childWidget);
    }
    return nullptr;
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
    QWidget::focusOutEvent(event);
}
