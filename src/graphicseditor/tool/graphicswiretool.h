#ifndef GRAPHICSWIRETOOL_H
#define GRAPHICSWIRETOOL_H

#include "abstractgraphicsinserttool.h"

class GraphicsWireItem;
class QAction;

class GraphicsWireTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    GraphicsWireTool(QObject *parent = 0);
    ~GraphicsWireTool();

private:
    QString m_toolGroup;
    QAction *m_toolAction;
    GraphicsWireItem *m_item;

    // GraphicsTool interface
public:
    virtual QDialog *optionDialog();
    virtual QString toolGroup() const;
    virtual QAction *action() const;
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

    // AbstractGraphicsInsertTool interface
public:
    virtual GraphicsObject *beginInsert(const QPointF &pos);
    virtual void addPoint(int idx, const QPointF &pos);
    virtual void freezePoint(int idx, const QPointF &pos);
    virtual bool removePoint(int idx, const QPointF &pos);
    virtual void movePoint(int idx, const QPointF &pos);
    virtual void endInsert(const QPointF &pos);
    virtual void cancelInsert();
};

#endif // GRAPHICSWIRETOOL_H
