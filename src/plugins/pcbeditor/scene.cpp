#include "scene.h"

#include <QPainter>

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
    init();
}

Scene::Scene(const QRectF &sceneRect, QObject *parent):
    QGraphicsScene(sceneRect, parent)
{
    init();
}

Scene::Scene(qreal x, qreal y, qreal width, qreal height, QObject *parent):
    QGraphicsScene(x, y, width, height, parent)
{
    init();
}

void Scene::init()
{
    m_cellSize.setHeight(100); // um
    m_cellSize.setWidth(100);  // um
}

// Efficiently draws a grid in the background.
// For more information: http://www.qtcentre.org/threads/5609-Drawing-grids-efficiently-in-QGraphicsScene?p=28905#post28905
void Scene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(sceneRect(), Qt::black);

    QRectF rect2 = rect.intersected(sceneRect());
    qreal left = int(rect2.left()) - (int(rect2.left()) % m_cellSize.width());
    qreal top = int(rect2.top()) - (int(rect2.top()) % m_cellSize.height());
    QVarLengthArray<QLineF, 100> lines;
    for (qreal x = left; x < rect2.right(); x += m_cellSize.width())
    {
        lines.append(QLineF(x, rect2.top(), x, rect2.bottom()));
    }
    for (qreal y = top; y < rect2.bottom(); y += m_cellSize.height())
    {
        lines.append(QLineF(rect2.left(), y, rect2.right(), y));
    }
    painter->setPen(QPen(Qt::lightGray, 0, Qt::SolidLine));
    painter->drawLines(lines.data(), lines.size());
}
