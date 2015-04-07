#include "widget/wireoperationwidget.h"
#include "widget/coordinatewidget.h"
#include "tool/graphicswiretool.h"
#include "abstractgraphicshandle.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

WireOperationWidget::WireOperationWidget(QWidget *parent) :
    QWidget(parent),
    m_tool(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    m_coordinateWidget = new CoordinateWidget;
    m_coordinateWidget->layout()->setMargin(0);
    layout->addWidget(m_coordinateWidget);

    m_undoButton = new QPushButton(QIcon(":/icons/task_undo.svg"), "&Undo");
    m_undoButton->setShortcut(QKeySequence("u"));
    layout->addWidget(m_undoButton);

    m_finishButton = new QPushButton(QIcon(":/icons/task_finish.svg"), "&Finish");
    m_finishButton->setShortcut(QKeySequence("f"));
    layout->addWidget(m_finishButton);

    m_closeButton = new QPushButton(QIcon(":/icons/task_close_shape.svg"), "&Close");
    m_closeButton->setShortcut(QKeySequence("c"));
    layout->addWidget(m_closeButton);

    m_wipeButton = new QPushButton(QIcon(":/icons/task_wipe.svg"), "&Wipe");
    m_wipeButton->setShortcut(QKeySequence("w"));
    layout->addWidget(m_wipeButton);

    // m_coordinateWidget will receive the focus instead of us
    setFocusProxy(m_coordinateWidget);
}

// FIXME: let the tool connect to us instead!
void WireOperationWidget::setTool(GraphicsWireTool *tool)
{
    if (m_tool != nullptr) {
        m_tool->disconnect(this);
    }

    m_tool = tool;

    if (m_tool != nullptr) {
        connect(m_coordinateWidget, &CoordinateWidget::coordinateChanged,
                [this](CoordinateWidget::Axis axis, qreal value) {
            qDebug() << "Changing axis value" << axis << value;
        });
        connect(m_coordinateWidget, &CoordinateWidget::coordinateEditingFinished,
                [this]() {
            qDebug() << "Adding point at" << m_coordinateWidget->coordinate();
            m_coordinateWidget->setFocus(Qt::TabFocusReason);
        });
        connect(m_undoButton, &QPushButton::pressed,
                [this]() {
            qDebug() << "Undoing";
        });
        connect(m_closeButton, &QPushButton::pressed,
                [this]() {
            qDebug() << "Closing";
        });
        connect(m_finishButton, &QPushButton::pressed,
                [this]() {
            qDebug() << "Finishing";
        });
        connect(m_wipeButton, &QPushButton::pressed,
                [this]() {
            qDebug() << "Wipping";
        });
    }
}

void WireOperationWidget::setCurrentLocation(const QPointF &pos)
{
    m_coordinateWidget->setCoordinate(pos);
}

void WireOperationWidget::focusInEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QWidget::focusInEvent(event);
}

void WireOperationWidget::focusOutEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QWidget::focusOutEvent(event);
}

bool WireOperationWidget::focusNextPrevChild(bool next)
{
    qDebug() << __PRETTY_FUNCTION__;
    return QWidget::focusNextPrevChild(next);
}
