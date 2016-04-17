#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class SchItem;

class SchScene : public QGraphicsScene
{
public:
    SchScene(QObject *parent);
    ~SchScene();

    QList<SchItem *> selectedObjects();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

};

#endif // GRAPHICSSCENE_H
