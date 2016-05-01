#ifndef GRAPHICSITEM_H
#define GRAPHICSITEM_H

#include <QGraphicsItem>
#include <QColor>
#include <QList>
#include <QPointF>
#include <QJsonObject>

class LayoutView;

class QWidget;
class QStyleOptionGraphicsItem;
class QPainter;

class GraphicsItem : public QGraphicsItem
{
public:
    explicit GraphicsItem(GraphicsItem *parent = nullptr);
    ~GraphicsItem();

    int layer() const;
    void setLayer(int layer);

    bool isLocked() const;
    void setLocked(bool locked);

    LayoutView *paintedView(QWidget *paintedWidget) const;
    bool shouldPaint(const QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *paintedWidget) const;
    QColor color(QWidget *paintedWidget) const;
    qreal opacity(QWidget *paintedWidget) const;

    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;

private:
    static const QString J_POSITION;
    static const QString J_ROTATION;
    static const QString J_LAYER;
    static const QString J_LOCKED;
    int m_layer;
    bool m_isLocked;
};

#endif // GRAPHICSITEM_H
