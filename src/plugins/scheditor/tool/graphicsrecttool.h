#ifndef GRAPHICSRECTTOOL_H
#define GRAPHICSRECTTOOL_H

#include "tool/abstractgraphicsinserttool.h"
#include <QRectF>

class GraphicsRectItem;

class GraphicsRectTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    GraphicsRectTool(QObject *parent);
    ~GraphicsRectTool();

protected:
    void setP1(const QPointF &pos);
    void setP2(const QPointF &pos);

private:
    GraphicsRectItem *m_item;
    QRectF m_rect;

    // GraphicsTool interface
public:
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

    // GraphicsTool interface
public slots:
    virtual void cancel();

    // AbstractGraphicsInsertTool interface
public:
    SchItem *beginInsert(const QPointF &pos);
    void addPoint(int idx, const QPointF &pos);
    void freezePoint(int idx, const QPointF &pos);
    bool removePoint(int idx, const QPointF &pos);
    void movePoint(int idx, const QPointF &pos);
    void endInsert(const QPointF &pos);
    void cancelInsert();
};

#endif // GRAPHICSRECTTOOL_H
