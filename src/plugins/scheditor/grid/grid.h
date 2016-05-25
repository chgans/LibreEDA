#pragma once

#include <QString>
#include <QPointF>
#include <QColor>
#include <QBrush>
#include <QRectF>
#include <QPainterPath>

class QPainter;

namespace SymbolEditor
{

    class Grid
    {
    public:

        explicit Grid();
        explicit Grid(const Grid &other);
        virtual ~Grid();

        QString label() const
        {
            return m_label;
        }
        void setLabel(const QString &label)
        {
            m_label = label;
        }

        virtual QString typeName() const = 0;
        virtual QString typeId() const = 0; // FIXME: see Factory.typeId()
        virtual QString description() const = 0;

        virtual Grid *clone() const = 0;

        virtual void draw(const QSizeF &pixelPerMm, QPainter *painter, const QRectF &rect) const = 0;
        virtual QRectF rect() const = 0;
        virtual QPainterPath shape(const QSizeF &pixelPerMm) const = 0;

        virtual QPointF snap(const QSizeF &pixelPerMm, QPointF point) const = 0;

        int priority() const
        {
            return m_priority;
        }
        void setPriority(int prio)
        {
            m_priority = prio;
        }

        QPointF origin() const
        {
            return m_origin;    // Why is this needed?
        }
        void setOrigin(QPointF p)
        {
            m_origin = p;
        }

        int minimalFeatureSize() const
        {
            return m_minimalFeatureSize;
        }
        void setMinimalFeatureSize(int pixels)
        {
            m_minimalFeatureSize = pixels;
        }

        qreal coarseMultiplier() const
        {
            return m_coarseMultiplier;
        }
        void setCoarseMultiplier(qreal m)
        {
            m_coarseMultiplier = m;
        }

        Qt::PenStyle fineLineStyle() const
        {
            return m_fineLineStyle;
        }
        void setFineLineStyle(Qt::PenStyle style)
        {
            m_fineLineStyle = style;
        }

        QColor fineLineColor() const
        {
            return m_fineLineColor;
        }
        void setFineLineColor(QColor color)
        {
            m_fineLineColor = color;
        }

        Qt::PenStyle coarseLineStyle() const
        {
            return m_coarseLineStyle;
        }
        void setCoarseLineStyle(Qt::PenStyle style)
        {
            m_coarseLineStyle = style;
        }

        QColor coarseLineColor() const
        {
            return m_coarseLineColor;
        }
        void setCoarseLineColor(QColor color)
        {
            m_coarseLineColor = color;
        }

    private:
        QString m_label;
        int m_priority;
        QPointF m_origin;
        int m_minimalFeatureSize;
        qreal m_coarseMultiplier;
        Qt::PenStyle m_fineLineStyle;
        QColor m_fineLineColor;
        Qt::PenStyle m_coarseLineStyle;
        QColor m_coarseLineColor;
    };

}
