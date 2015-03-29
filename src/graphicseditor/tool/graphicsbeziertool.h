#ifndef GRAPHICSBEZIERTOOL_H
#define GRAPHICSBEZIERTOOL_H

#include "graphicseditor/abstractgraphicsinserttool.h"

#include <QPointF>

class GraphicsBezierItem;

class GraphicsBezierTool : public AbstractGraphicsInsertTool
{
public:
    GraphicsBezierTool(QObject *parent);
    ~GraphicsBezierTool();

private:
    QAction *m_action;
    QString m_toolGroup;
    GraphicsBezierItem *m_item;

    // GraphicsTool interface
public:
    virtual QDialog *optionDialog();
    virtual QString toolGroup() const;
    virtual QAction *action() const;
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

    // AbstractGraphicsInsertTool interface
public:
    GraphicsObject *beginInsert(const QPointF &pos);
    void addPoint(int idx, const QPointF &pos);
    void freezePoint(int idx, const QPointF &pos);
    bool removePoint(int idx, const QPointF &pos);
    void movePoint(int idx, const QPointF &pos);
    void endInsert(const QPointF &pos);
    void cancelInsert();
};

#endif // GRAPHICSBEZIERTOOL_H
