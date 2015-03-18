#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class GraphicsObject;
class GraphicsGrid;

class GraphicsScene : public QGraphicsScene
{
public:
    GraphicsScene(QObject *parent);
    ~GraphicsScene();

    QList<GraphicsObject *> selectedObjects();

    GraphicsGrid *grid();
    void setGrid(GraphicsGrid *grid);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:

    GraphicsGrid *m_grid;
};

#endif // GRAPHICSSCENE_H
