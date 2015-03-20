#ifndef GRAPHICSRECTTOOL_H
#define GRAPHICSRECTTOOL_H

#include "graphicseditor/abstractgraphicsinserttool.h"

class GraphicsRectItem;

class GraphicsRectTool : public AbstractGraphicsInsertTool
{
    Q_OBJECT

public:
    GraphicsRectTool(QObject *parent);
    ~GraphicsRectTool();

    // GraphicsTool interface
public:
    virtual QDialog *optionDialog();
    virtual QString toolGroup() const;
    virtual QAction *action() const;
    virtual void activate(const QAction *which);
    virtual void desactivate(const QAction *which);

    // GraphicsTool interface
public slots:
    virtual void cancel();

    // AbstractGraphicsInsertTool interface
public:
    GraphicsObject *beginInsert(const QPointF &pos);
    void addPoint(int idx, const QPointF &pos);
    void freezePoint(int idx);
    bool removePoint(int idx);
    void movePoint(int idx, const QPointF &pos);
    void endInsert(const QPointF &pos);
    void cancelInsert();


protected:
    void setP1(const QPointF &pos);
    void setP2(const QPointF &pos);

private:
    QString m_toolGroup;
    QAction *m_toolAction;
    GraphicsRectItem *m_item;
};

#endif // GRAPHICSRECTTOOL_H
