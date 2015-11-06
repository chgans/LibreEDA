#ifndef RECTANGLEDATA_H
#define RECTANGLEDATA_H

#include "document/graphicsitemdata.h"

class RectangleData: public GraphicsItemData
{
    Q_GADGET

    Q_PROPERTY(QPointF topLeft READ topLeft WRITE setTopLeft)
    Q_PROPERTY(QSizeF size READ size WRITE setSize)

    QPointF m_topLeft;
    QSizeF m_size;

public:
    RectangleData();

    QPointF topLeft() const
    {
        return m_topLeft;
    }

    QSizeF size() const
    {
        return m_size;
    }

public slots:
    void setTopLeft(QPointF topLeft)
    {
        m_topLeft = topLeft;
    }

    void setSize(QSizeF size)
    {
        m_size = size;
    }
};

#endif // RECTANGLEDATA_H
