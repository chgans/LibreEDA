#ifndef GRAPHICSLINETOOL_H
#define GRAPHICSLINETOOL_H

#include "tool/abstractgraphicsinserttool.h"

class GraphicsLineItem;
class PenSettingsWidget;

class GraphicsLineTool : public AbstractGraphicsInsertTool
{
public:
    GraphicsLineTool(QObject *parent = 0);

private:
    GraphicsLineItem *m_item;
    PenSettingsWidget *m_penSettingsWidget;

private:
    void setP1(const QPointF &pos);
    void setP2(const QPointF &pos);

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

#endif // GRAPHICSLINETOOL_H
