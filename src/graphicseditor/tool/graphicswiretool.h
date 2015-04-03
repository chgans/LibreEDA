#ifndef GRAPHICSWIRETOOL_H
#define GRAPHICSWIRETOOL_H

#include "abstractgraphicsinserttool.h"

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
    QString m_toolGroup;
    QAction *m_toolAction;
    GraphicsWireItem *m_item;
    WireOperationWidget *m_operationWidget;
    PenSettingsWidget *m_optionWidget;

    // GraphicsTool interface
public:
    virtual QWidget *taskWidget();
    virtual QWidget *optionWidget();
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
