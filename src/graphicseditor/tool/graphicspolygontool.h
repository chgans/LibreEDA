#ifndef GRAPHICSPOLYGONTOOL_H
#define GRAPHICSPOLYGONTOOL_H

#include "abstractgraphicsinserttool.h"

class GraphicsPolygonItem;
class QAction;

class GraphicsPolygonTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    GraphicsPolygonTool(QObject *parent = 0);
    ~GraphicsPolygonTool();

private:
    QAction *m_action;
    QString m_toolGroup;
    GraphicsPolygonItem *m_item;


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

#endif // GRAPHICSPOLYGONTOOL_H
