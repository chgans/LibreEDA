#include "schscene.h"
#include "schitem.h"
#include "abstractgraphicshandle.h"
#include "graphicsgrid.h"

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
    painter->fillRect(sceneRect().intersected(rect), QColor::fromRgb(0xff, 0xfc, 0xf8));
}

