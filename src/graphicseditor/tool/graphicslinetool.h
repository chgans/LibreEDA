#ifndef GRAPHICSLINETOOL_H
#define GRAPHICSLINETOOL_H

#include "graphicseditor/abstractgraphicsinserttool.h"

class GraphicsLineItem;

class GraphicsLineTool : public AbstractGraphicsInsertTool
{
public:
    GraphicsLineTool(QObject *parent = 0);

    // GraphicsTool interface
public:
    virtual QDialog *optionDialog();
    virtual QString toolGroup() const;
    virtual QAction *action() const;

    // GraphicsTool interface
public slots:
    virtual void cancel();

    // GraphicsTool interface
public:
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

private:
    GraphicsLineItem *m_item;

    // AbstractGraphicsInsertTool interface
public:
    GraphicsObject *beginInsert(const QPointF &pos);
    void addPoint(int idx, const QPointF &pos);
    void freezePoint(int idx);
    bool removePoint(int idx);
    void movePoint(int idx, const QPointF &pos);
    void endInsert(const QPointF &pos);
    void cancelInsert();

private:
    QString m_toolGroup;
    QAction *m_action;

    void setP1(const QPointF &pos);
    void setP2(const QPointF &pos);
};

#endif // GRAPHICSLINETOOL_H
