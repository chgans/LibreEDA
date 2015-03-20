#ifndef ABSTRACTGRAPHICSINTERACTIVETOOL_H
#define ABSTRACTGRAPHICSINTERACTIVETOOL_H

#include "graphicstool.h"

class QMouseEvent;
class QKeyEvent;
class QWheelEvent;

class GraphicsObject;

class AbstractGraphicsInteractiveTool: public GraphicsTool
{
    Q_OBJECT
public:
    AbstractGraphicsInteractiveTool(QObject *parent = 0);
    ~AbstractGraphicsInteractiveTool();

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void wheelEvent(QWheelEvent *event);

protected:
    GraphicsObject *createPhantomItem(GraphicsObject *item);
    QList<GraphicsObject *> createPhantomItems(const QList<GraphicsObject *> &items);
};

#endif // ABSTRACTGRAPHICSINTERACTIVETOOL_H
