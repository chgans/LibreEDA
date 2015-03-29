#ifndef GRAPHICSELLIPSETOOL_H
#define GRAPHICSELLIPSETOOL_H

#include "abstractgraphicsinserttool.h"

class GraphicsEllipseItem;

class QAction;

class GraphicsEllipseTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    GraphicsEllipseTool(QObject *parent = 0);
    ~GraphicsEllipseTool();

private:
    QAction *m_action;
    QString m_toolGroup;
    GraphicsEllipseItem *m_item;

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

#endif // GRAPHICSELLIPSETOOL_H
