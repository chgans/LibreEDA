#ifndef GRAPHICSCARTESIANGRID_H
#define GRAPHICSCARTESIANGRID_H

#include "grid/graphicsgrid.h"
#include <QPointF>
#include <QSizeF>

class GraphicsCartesianGrid : public GraphicsGrid
{
public:
    enum Quadrant
    {
        TopLeftQuadrant = 0,
        TopRightQuadrant = 1,
        BottomRightQuadrant = 2,
        BottomLeftQuadrant = 3
    };

    explicit GraphicsCartesianGrid();
    explicit GraphicsCartesianGrid(const GraphicsCartesianGrid &other);

    QString typeName() const;
    QString typeId() const;
    QString description() const;
    GraphicsGrid *clone() const;
    void draw(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &rect) const;
    QRectF rect() const;
    QPointF snap(const QSizeF &pixelPerMm, QPointF point) const;
    QPainterPath shape(const QSizeF &pixelPerMm) const;

    qreal rotation() const
    {
        return mRotation;
    }
    void setRotation(qreal angle)
    {
        mRotation = angle;
    }

    QSizeF size() const
    {
        return mSize;
    }
    void setSize(QSizeF size)
    {
        mSize = size;
    }

    QPointF step() const
    {
        return mStep;
    }
    void setStep(QPointF step)
    {
        mStep = step;
    }

    bool isQuadrantEnabled(Quadrant quadrant) const
    {
        return mQuadrants[quadrant];
    }
    void setQuadrantEnabled(Quadrant quadrant, bool enabled)
    {
        mQuadrants[quadrant] = enabled;
    }


protected:
    void drawGrid(QPainter *painter, const QColor &color, Qt::PenStyle style,
                  const QRectF &rect, qreal xstep, qreal ystep) const;
    void drawQuadrantGrid(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &rect) const;

    bool shouldDrawCoarse(const QSizeF &pixelPerMm) const;
    bool shouldDrawFine(const QSizeF &pixelPerMm) const;

    QRectF quadrantRect(Quadrant which) const;

    qreal mRotation;
    QSizeF mSize;
    QPointF mStep; // not used!?!
    bool mQuadrants[4];
};

#endif // GRAPHICSCARTESIANGRID_H
