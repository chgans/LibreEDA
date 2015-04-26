#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class SchItem;
class GraphicsGrid;

class SchScene : public QGraphicsScene
{
public:
    SchScene(QObject *parent);
    ~SchScene();

    QList<SchItem *> selectedObjects();

    GraphicsGrid *grid();
    void setGrid(GraphicsGrid *grid);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

    GraphicsGrid *m_grid;
};

#endif // GRAPHICSSCENE_H
