#include "abstractgraphicsinteractivetool.h"

#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicsobject.h"

#include <QGraphicsColorizeEffect>

#include <QMouseEvent>
#include <QKeyEvent>

AbstractGraphicsInteractiveTool::AbstractGraphicsInteractiveTool(QObject *parent):
    GraphicsTool(parent)
{

}

AbstractGraphicsInteractiveTool::~AbstractGraphicsInteractiveTool()
{

}

QWidget *AbstractGraphicsInteractiveTool::optionWidget()
{
    return m_optionWidget;
}

void AbstractGraphicsInteractiveTool::setOptionWidget(QWidget *widget)
{
    m_optionWidget = widget;
}

QWidget *AbstractGraphicsInteractiveTool::taskWidget()
{
    return m_taskWidget;
}

void AbstractGraphicsInteractiveTool::setOperationWidget(QWidget *widget)
{
    m_taskWidget = widget;
}

void AbstractGraphicsInteractiveTool::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void AbstractGraphicsInteractiveTool::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void AbstractGraphicsInteractiveTool::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void AbstractGraphicsInteractiveTool::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void AbstractGraphicsInteractiveTool::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void AbstractGraphicsInteractiveTool::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void AbstractGraphicsInteractiveTool::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}

GraphicsObject *AbstractGraphicsInteractiveTool::createPhantomItem(GraphicsObject *item)
{
    GraphicsObject *phantomItem = item->clone();
    // Pity we cannot mix effects, drop shadow is nice while moving/clonig
    // But not good while moving handles with the select tool
    QGraphicsColorizeEffect *phantomEffect = new QGraphicsColorizeEffect();
    phantomEffect->setColor(Qt::darkGray);
    phantomEffect->setStrength(.5);
    phantomEffect->setEnabled(true);
    phantomItem->setOpacity(phantomItem->opacity()*0.9);
    phantomItem->setGraphicsEffect(phantomEffect);
    scene()->addItem(phantomItem);
    phantomItem->setSelected(false); // force deselected
    return phantomItem;
}

QList<GraphicsObject *> AbstractGraphicsInteractiveTool::createPhantomItems(const QList<GraphicsObject *> &items)
{
    QList<GraphicsObject *> phantomItems;
    foreach (GraphicsObject *item, items) {
        phantomItems.append(createPhantomItem(item));
    }
    return phantomItems;
}
