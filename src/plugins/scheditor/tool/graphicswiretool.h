#ifndef GRAPHICSWIRETOOL_H
#define GRAPHICSWIRETOOL_H

#include "tool/abstractgraphicsinserttool.h"

class PenSettingsWidget;
class WireOperationWidget;
class GraphicsWireItem;
class QAction;
class QLineEdit;

class GraphicsWireTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    GraphicsWireTool(QObject *parent = 0);
    ~GraphicsWireTool();

private:
    GraphicsWireItem *m_item;

    // GraphicsTool interface
public:
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

    // AbstractGraphicsInsertTool interface
public:
    virtual SchItem *beginInsert(const QPointF &pos);
    virtual void addPoint(int idx, const QPointF &pos);
    virtual void freezePoint(int idx, const QPointF &pos);
    virtual bool removePoint(int idx, const QPointF &pos);
    virtual void movePoint(int idx, const QPointF &pos);
    virtual void endInsert(const QPointF &pos);
    virtual void cancelInsert();
};

#endif // GRAPHICSWIRETOOL_H
