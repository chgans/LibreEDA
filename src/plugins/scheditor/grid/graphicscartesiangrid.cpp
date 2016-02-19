#include "grid/graphicscartesiangrid.h"

#include <QLineF>
#include <QPainter>
#include <QPainterPath>
#include <cmath>

//
static const qreal MIN_SIZE_IN_PIXEL = 5;

GraphicsCartesianGrid::GraphicsCartesianGrid():
    GraphicsGrid(),
    mRotation(0),
    mSize(100, 100),
    mStep(10, 10)
{
    for (int i=0; i<4; ++i)
        mQuadrants[i] = true;
}

GraphicsCartesianGrid::GraphicsCartesianGrid(const GraphicsCartesianGrid &other):
    GraphicsGrid(other),
    mRotation(other.mRotation),
    mSize(other.mSize),
    mStep(other.mStep)
{
    for (int i=0; i<4; ++i)
        mQuadrants[i] = other.mQuadrants[i];
}

QString GraphicsCartesianGrid::typeName() const
{
    return "Cartesian";
}

QString GraphicsCartesianGrid::typeId() const
{
    // FIXME: same as in factory
    return QStringLiteral("LibreEDA.Grid.Cartesian");
}

QString GraphicsCartesianGrid::description() const
{
    return QString("Origin(%1, %2), Size(%3, %4), Steps(%5, %6)")
            .arg(origin().x()).arg(origin().y())
            .arg(size().width()).arg(size().height())
            .arg(step().x()).arg(step().y());
}

GraphicsGrid *GraphicsCartesianGrid::clone() const
{
    GraphicsCartesianGrid *grid = new GraphicsCartesianGrid(*this);
    return grid;
}

QRectF GraphicsCartesianGrid::rect() const
{
    return QRectF(origin(), size());
}

QPointF GraphicsCartesianGrid::snap(const QSizeF &pixelPerMm, QPointF point) const
{
    if (point.isNull() || step().isNull())
        return QPointF();

    qreal xstep = step().x();
    qreal ystep = step().y();
    if (shouldDrawFine(pixelPerMm)) {
        xstep /= coarseMultiplier();
        ystep /= coarseMultiplier();
    }

    qreal x = qRound((point.x() - origin().x())/ xstep) * xstep + origin().x();
    qreal y = qRound((point.y() - origin().y())/ ystep) * ystep + origin().y();
    return QPointF(x, y);
}

QPainterPath GraphicsCartesianGrid::shape(const QSizeF &pixelPerMm) const
{
    QPainterPath path;

    if (!shouldDrawCoarse(pixelPerMm))
        return path;

    for (int q = TopLeftQuadrant; q <= BottomLeftQuadrant; q ++)
        path.addRect(quadrantRect(Quadrant(q)));

    return path;
}


void GraphicsCartesianGrid::draw(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &prect) const
{
    bool drawCoarse = shouldDrawCoarse(pixelPerMm);
    if (!drawCoarse)
        return;

    for (int q = TopLeftQuadrant; q <= BottomLeftQuadrant; q ++)
        drawQuadrantGrid(pixelPerMm, painter, quadrantRect(Quadrant(q)).intersected(prect));
}


void GraphicsCartesianGrid::drawQuadrantGrid(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &rect) const
{
    bool drawFine = shouldDrawFine(pixelPerMm);

    if (!rect.isEmpty()) {
        if (drawFine)
            drawGrid(painter, fineLineColor(), fineLineStyle(),
                         rect, step().x()/coarseMultiplier(), step().y()/coarseMultiplier());
        drawGrid(painter, coarseLineColor(), coarseLineStyle(),
                     rect, step().x(), step().y());
    }
}

void GraphicsCartesianGrid::drawGrid(QPainter *painter, const QColor &color, Qt::PenStyle style,
                                 const QRectF &rect, qreal xstep, qreal ystep) const
{
    qreal left = int((rect.left() - origin().x())/step().x()) * step().x() + origin().x();
    qreal top = int((rect.top()  - origin().y())/step().y()) * step().y() + origin().y();
    qreal right = rect.right();
    qreal bottom = rect.bottom();

    painter->setPen(QPen(color, 0, style));
    if (style == Qt::SolidLine) {
        QVarLengthArray<QLineF, 1024> lines;
        for (qreal x = left; x <= right; x += xstep) {
            lines.append(QLineF(x, top,
                                x, bottom));
        }
        for (qreal y = top; y <= bottom; y += ystep) {
            lines.append(QLineF(left, y,
                                right, y));
        }
        painter->drawLines(lines.data(), lines.size());
    }
    else if (style == Qt::DotLine) {
        QVarLengthArray<QPointF, 1024> points;
        for (qreal x = left; x <= right; x += xstep) {
            for (qreal y = top; y <= bottom; y += ystep) {
                points.append(QPointF(x, y));
            }
        }
        painter->drawPoints(points.data(), points.size());
    }
}

bool GraphicsCartesianGrid::shouldDrawCoarse(const QSizeF &pixelPerMm) const
{
    return pixelPerMm.width()*step().x() > MIN_SIZE_IN_PIXEL &&
            pixelPerMm.height()*step().y() > MIN_SIZE_IN_PIXEL;
}

bool GraphicsCartesianGrid::shouldDrawFine(const QSizeF &pixelPerMm) const
{
    return (pixelPerMm.width()*step().x()/coarseMultiplier()) > MIN_SIZE_IN_PIXEL &&
            (pixelPerMm.height()*step().y()/coarseMultiplier()) > MIN_SIZE_IN_PIXEL;
}

QRectF GraphicsCartesianGrid::quadrantRect(GraphicsCartesianGrid::Quadrant which) const
{
    if (which == TopLeftQuadrant && mQuadrants[which])
        return QRectF(rect().topLeft(),
                      rect().center());
    if (which == TopRightQuadrant && mQuadrants[which])
        return QRectF(QPointF(rect().center().x(), rect().top()),
                      QPointF(rect().right(), rect().center().y()));
    if (which == BottomRightQuadrant && mQuadrants[which])
        return QRectF(rect().center(),
                      rect().bottomRight());
    if (which == BottomLeftQuadrant && mQuadrants[which])
        return QRectF(QPointF(rect().left(), rect().center().y()),
                      QPointF(rect().center().x(), rect().bottom()));
    return QRectF();
}
