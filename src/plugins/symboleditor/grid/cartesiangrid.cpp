#include "grid/cartesiangrid.h"

#include <QLineF>
#include <QPainter>
#include <QPainterPath>
#include <cmath>

namespace SymbolEditor
{

    CartesianGrid::CartesianGrid():
        Grid(),
        mRotation(0),
        mSize(100, 100),
        mStep(10, 10)
    {
        for (int i = 0; i < 4; ++i)
        {
            mQuadrants[i] = true;
        }
    }

    CartesianGrid::CartesianGrid(const CartesianGrid &other):
        Grid(other),
        mRotation(other.mRotation),
        mSize(other.mSize),
        mStep(other.mStep)
    {
        for (int i = 0; i < 4; ++i)
        {
            mQuadrants[i] = other.mQuadrants[i];
        }
    }

    QString CartesianGrid::typeName() const
    {
        return "Cartesian";
    }

    QString CartesianGrid::typeId() const
    {
        // FIXME: same as in factory
        return QStringLiteral("LibreEDA.Grid.Cartesian");
    }

    QString CartesianGrid::description() const
    {
        return QString("Origin(%1, %2), Size(%3, %4), Steps(%5, %6)")
                .arg(origin().x()).arg(origin().y())
                .arg(size().width()).arg(size().height())
                .arg(step().x()).arg(step().y());
    }

    Grid *CartesianGrid::clone() const
    {
        CartesianGrid *grid = new CartesianGrid(*this);
        return grid;
    }

    QRectF CartesianGrid::rect() const
    {
        return QRectF(origin(), size());
    }

    QPointF CartesianGrid::snap(const QSizeF &pixelPerMm, QPointF point) const
    {
        if (point.isNull() || step().isNull())
        {
            return QPointF();
        }

        qreal xstep = step().x();
        qreal ystep = step().y();
        if (shouldDrawFine(pixelPerMm))
        {
            xstep /= coarseMultiplier();
            ystep /= coarseMultiplier();
        }

        qreal x = qRound((point.x() - origin().x()) / xstep) * xstep + origin().x();
        qreal y = qRound((point.y() - origin().y()) / ystep) * ystep + origin().y();
        return QPointF(x, y);
    }

    QPainterPath CartesianGrid::shape(const QSizeF &pixelPerMm) const
    {
        QPainterPath path;

        if (!shouldDrawCoarse(pixelPerMm))
        {
            return path;
        }

        for (int q = TopLeftQuadrant; q <= BottomLeftQuadrant; q ++)
        {
            path.addRect(quadrantRect(Quadrant(q)));
        }

        return path;
    }


    void CartesianGrid::draw(const QSizeF &pixelPerMm, QPainter *painter,
                             const QRectF &prect) const
    {
        bool drawCoarse = shouldDrawCoarse(pixelPerMm);
        if (!drawCoarse)
        {
            return;
        }

        for (int q = TopLeftQuadrant; q <= BottomLeftQuadrant; q ++)
        {
            drawQuadrantGrid(pixelPerMm, painter, quadrantRect(Quadrant(q)).intersected(prect));
        }
    }


    void CartesianGrid::drawQuadrantGrid(const QSizeF &pixelPerMm, QPainter *painter,
                                         const QRectF &rect) const
    {
        bool drawFine = shouldDrawFine(pixelPerMm);

        if (!rect.isEmpty())
        {
            drawGrid(painter, coarseLineColor(), coarseLineStyle(),
                     rect, step().x(), step().y());
            if (drawFine)
                drawGrid(painter, fineLineColor(), fineLineStyle(),
                         rect, step().x() / coarseMultiplier(), step().y() / coarseMultiplier());
        }
    }

    void CartesianGrid::drawGrid(QPainter *painter, const QColor &color, Qt::PenStyle style,
                                 const QRectF &rect, qreal xstep, qreal ystep) const
    {
        qreal left = int((rect.left() - origin().x()) / step().x()) * step().x() + origin().x();
        qreal top = int((rect.top()  - origin().y()) / step().y()) * step().y() + origin().y();
        qreal right = rect.right();
        qreal bottom = rect.bottom();

        painter->setPen(QPen(color, 0, style));
        if (style == Qt::SolidLine)
        {
            QVarLengthArray<QLineF, 1024> lines;
            for (qreal x = left; x <= right; x += xstep)
            {
                lines.append(QLineF(x, top,
                                    x, bottom));
            }
            for (qreal y = top; y <= bottom; y += ystep)
            {
                lines.append(QLineF(left, y,
                                    right, y));
            }
            painter->drawLines(lines.data(), lines.size());
        }
        else if (style == Qt::DotLine)
        {
            QVarLengthArray<QPointF, 1024> points;
            for (qreal x = left; x <= right; x += xstep)
            {
                for (qreal y = top; y <= bottom; y += ystep)
                {
                    points.append(QPointF(x, y));
                }
            }
            painter->drawPoints(points.data(), points.size());
        }
    }

    bool CartesianGrid::shouldDrawCoarse(const QSizeF &pixelPerMm) const
    {
        return pixelPerMm.width() * step().x() > minimalFeatureSize() &&
                pixelPerMm.height() * step().y() > minimalFeatureSize();
    }

    bool CartesianGrid::shouldDrawFine(const QSizeF &pixelPerMm) const
    {
        return (pixelPerMm.width() * step().x() / coarseMultiplier()) > minimalFeatureSize() &&
                (pixelPerMm.height() * step().y() / coarseMultiplier()) > minimalFeatureSize();
    }

    QRectF CartesianGrid::quadrantRect(CartesianGrid::Quadrant which) const
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

}
