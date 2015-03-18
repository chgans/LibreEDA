#include "graphicsscene.h"
#include "graphicsobject.h"
#include "graphicshandle.h"
#include "graphicsgrid.h"

#include <QPainter>

GraphicsScene::GraphicsScene(QObject *parent):
    QGraphicsScene(parent), m_grid(nullptr)
{
}

GraphicsScene::~GraphicsScene()
{

}

QList<GraphicsObject *> GraphicsScene::selectedObjects()
{
    QList<GraphicsObject *> objects;
    foreach (QGraphicsItem *item, selectedItems()) {
        GraphicsObject *object = dynamic_cast<GraphicsObject *>(item);
        if (object != nullptr)
            objects.append(object);
    }
    return objects;
}

GraphicsGrid *GraphicsScene::grid()
{
    return m_grid;
}

void GraphicsScene::setGrid(GraphicsGrid *grid)
{
    m_grid = grid;
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(sceneRect().intersected(rect), Qt::black);
}

