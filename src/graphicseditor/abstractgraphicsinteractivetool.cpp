#include "abstractgraphicsinteractivetool.h"

#include "graphicsscene.h"
#include "graphicsview.h"
#include "graphicsobject.h"

#include <QGraphicsOpacityEffect>

#include <QMouseEvent>
#include <QKeyEvent>

AbstractGraphicsInteractiveTool::AbstractGraphicsInteractiveTool(QObject *parent):
    GraphicsTool(parent)
{

}

AbstractGraphicsInteractiveTool::~AbstractGraphicsInteractiveTool()
{

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
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    effect->setEnabled(true);
    phantomItem->setGraphicsEffect(effect);
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

