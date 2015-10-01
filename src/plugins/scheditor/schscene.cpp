#include "schscene.h"
#include "item/schitem.h"
#include "handle/abstractgraphicshandle.h"
#include "grid/graphicsgrid.h"
#include "palette.h"

#include <QPainter>

SchScene::SchScene(QObject *parent):
    QGraphicsScene(parent), m_grid(nullptr)
{
}

SchScene::~SchScene()
{

}

QList<SchItem *> SchScene::selectedObjects()
{
    QList<SchItem *> objects;
    foreach (QGraphicsItem *item, selectedItems()) {
        SchItem *object = dynamic_cast<SchItem *>(item);
        if (object != nullptr)
            objects.append(object);
    }
    return objects;
}

GraphicsGrid *SchScene::grid()
{
    return m_grid;
}

void SchScene::setGrid(GraphicsGrid *grid)
{
    m_grid = grid;
}

void SchScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    // FIXME: Get access to the view's palette
    //painter->setPen(QPen(gpalette::Content1, 0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(backgroundBrush());
    painter->drawRect(sceneRect());
}

