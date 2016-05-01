#ifndef GRAPHICSITEMDATA_H
#define GRAPHICSITEMDATA_H

#include "document/schdocumentobject.h"

#include <QPen>
#include <QBrush>
#include <QRectF>

class GraphicsItemData: public SchDocumentObject
{
    Q_GADGET
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QPointF position READ position WRITE setPosition)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    Q_PROPERTY(qreal zValue READ zValue WRITE setZValue)
//    Q_PROPERTY(bool locked READ isLocked WRITE setLocked)
//    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
//    Q_PROPERTY(bool hMirrored READ isHMirrored WRITE setHMirrored)
//    Q_PROPERTY(bool vMirrored READ isVMirrored WRITE setVMirrored)

    // group vs child items
    // flags?
    // enable, selected

    QPen m_pen;
    QBrush m_brush;
    qreal m_opacity;
    QPointF m_position;
    qreal m_rotation;
    qreal m_scale;
    qreal m_zValue;
    bool m_locked;
    bool m_visible;
    bool m_hMirrored;
    bool m_vMirrored;

public:
    explicit GraphicsItemData(SchDocumentObject *parent = nullptr);

    QPen pen() const
    {
        return m_pen;
    }

    QBrush brush() const
    {
        return m_brush;
    }

    qreal opacity() const
    {
        return m_opacity;
    }

    QPointF position() const
    {
        return m_position;
    }

    qreal rotation() const
    {
        return m_rotation;
    }

    qreal scale() const
    {
        return m_scale;
    }

    qreal zValue() const
    {
        return m_zValue;
    }

public slots:
    void setPen(QPen pen)
    {
        m_pen = pen;
    }

    void setBrush(QBrush brush)
    {
        m_brush = brush;
    }

    void setOpacity(qreal opacity)
    {
        m_opacity = opacity;
    }

    void setPosition(QPointF position)
    {
        m_position = position;
    }

    void setRotation(qreal rotation)
    {
        m_rotation = rotation;
    }

    void setScale(qreal scale)
    {
        m_scale = scale;
    }

    void setZValue(qreal zValue)
    {
        m_zValue = zValue;
    }
};

#endif // GRAPHICSITEMDATA_H
