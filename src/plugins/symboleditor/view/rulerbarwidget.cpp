#include "rulerbarwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QFontMetrics>

#include <QtMath>

// https://kernelcoder.wordpress.com/2010/08/25/how-to-insert-ruler-scale-type-widget-into-a-qabstractscrollarea-type-widget/
// http://stackoverflow.com/questions/10027008/separate-layers-like-feature-on-qgraphicsview

namespace SymbolEditor
{

    const int RulerBarWidget::BREADTH = 23;

    RulerBarWidget::RulerBarWidget(RulerBarWidget::Alignment alignment, QWidget *parent) :
        QWidget(parent), m_alignment(alignment)
    {
    }

    RulerBarWidget::~RulerBarWidget()
    {

    }

    void RulerBarWidget::setCursorRange(qreal first, qreal last)
    {
        m_firstPos = first;
        m_lastPos = last;
        update();
    }

    void RulerBarWidget::setCursorPosition(const QPointF &pos)
    {
        if (m_alignment == Horizontal)
        {
            m_currentPos = pos.x();
        }
        else
        {
            m_currentPos = pos.y();
        }
        update();
    }

    QSize RulerBarWidget::minimumSizeHint() const
    {
        return QSize(BREADTH, BREADTH);
    }

    RulerBarWidget::Alignment RulerBarWidget::rulerType() const
    {
        return m_alignment;
    }

    void RulerBarWidget::setBackgroundColor(const QColor &color)
    {
        if (m_backgroundColor == color)
        {
            return;
        }
        m_backgroundColor = color;
        update();
    }

    QColor RulerBarWidget::backgroundColor() const
    {
        return m_backgroundColor;
    }

    void RulerBarWidget::setForegroundColor(const QColor &color)
    {
        if (m_foregroundColor == color)
        {
            return;
        }
        m_foregroundColor = color;
        update();
    }

    QColor RulerBarWidget::foregroundColor() const
    {
        return m_foregroundColor;
    }

    static const int FONT_SIZE = 10;
    static const int MAJOR_TICK_HEIGHT = 6;
    static const int MINOR_TICK_HEIGHT = 3;

    // TODO: update only inside event->rect()
    void RulerBarWidget::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setClipRect(event->rect());
        painter.fillRect(event->rect(), m_backgroundColor);
        QPen pen(m_foregroundColor, 0);
        QBrush brush(m_foregroundColor);
        painter.setPen(pen);
        painter.setBrush(brush);

        int minLocalStep = 50; // minimum 10 pixel between any 2 ticks
        int nbLocalStep = length() / minLocalStep;
        if (nbLocalStep <= 0)
        {
            return;
        }
        qreal logicalRange = (m_lastPos - m_firstPos);
        qreal logicalStep = logicalRange / nbLocalStep;
        qreal logicalMinorStep = std::pow(10.0f, floor(log10(logicalStep)));
        static const int minorTickMultiplier = 5;
        qreal logicalMajorStep = minorTickMultiplier * logicalMinorStep;

        qreal ratio = length() / (m_lastPos - m_firstPos);
        int nbLogicalSteps = (m_lastPos - m_firstPos) / logicalMinorStep;
        if (nbLogicalSteps <= 0)
        {
            return;
        }
        qreal firstLogicalMinorPoint = m_firstPos - fmod(m_firstPos, logicalMinorStep) + logicalMinorStep;
        qreal firstLogicalMajorPoint = m_firstPos - fmod(m_firstPos, logicalMajorStep) + logicalMajorStep;
        QVector<qreal> logicalPoints(nbLogicalSteps);
        QVector<qreal> localPoints(nbLogicalSteps);
        for (int i = 0; i < nbLogicalSteps; i++)
        {
            logicalPoints[i] = firstLogicalMinorPoint + logicalMinorStep * i;
            localPoints[i] = (logicalPoints[i] - m_firstPos) * ratio;
            if (qFuzzyCompare(logicalPoints[i], firstLogicalMajorPoint))
            {
                drawMajorTick(painter, localPoints[i], logicalPoints[i]);
                firstLogicalMajorPoint += logicalMajorStep;
            }
            else
            {
                drawMinorTick(painter, localPoints[i]);
            }
        }
        drawIndicator(painter, ratio * (m_currentPos - m_firstPos));
    }

    int RulerBarWidget::length()
    {
        if (m_alignment == Vertical)
        {
            return height();
        }
        else
        {
            return width();
        }
    }

    void RulerBarWidget::drawMajorTick(QPainter &painter, int pixelPos, qreal logicalPos)
    {
        QFont font = painter.font();
        font.setPixelSize(FONT_SIZE);
        painter.setFont(font);
        QString text = QString("%1").arg(logicalPos);
        QFontMetrics fontMetrics(font);
        int textWidth = fontMetrics.width(text);
        if (m_alignment == Horizontal)
        {
            painter.drawText(QPointF(pixelPos - textWidth / 2, FONT_SIZE + 2), text);
            painter.drawLine(QPointF(pixelPos, (BREADTH - 1) - MAJOR_TICK_HEIGHT),
                             QPointF(pixelPos, (BREADTH - 1)));
        }
        else
        {
            painter.save();
            painter.translate(FONT_SIZE + 2, pixelPos + textWidth / 2);
            painter.rotate(-90);
            painter.drawText(0, 0, text);
            painter.restore();
            painter.drawLine(QPointF((BREADTH - 1) - MAJOR_TICK_HEIGHT, pixelPos),
                             QPointF((BREADTH - 1),                     pixelPos));
        }
    }

    void RulerBarWidget::drawMinorTick(QPainter &painter, int pixelPos)
    {
        if (m_alignment == Horizontal)
        {
            painter.drawLine(QPointF(pixelPos, (BREADTH - 1) - MINOR_TICK_HEIGHT),
                             QPointF(pixelPos, (BREADTH - 1)));
        }
        else
        {
            painter.drawLine(QPointF((BREADTH - 1) - MINOR_TICK_HEIGHT, pixelPos),
                             QPointF((BREADTH - 1),                     pixelPos));
        }
    }

    void RulerBarWidget::drawIndicator(QPainter &painter, int pixelPos)
    {
        static const int cursorWidth = 6;
        static const int cursorHeight = 3;

        QPolygon shape;
        if (m_alignment == Horizontal)
        {
            shape << QPoint(-(cursorWidth >> 1), 0)
                  << QPoint(+(cursorWidth >> 1), 0)
                  << QPoint(0,                 cursorHeight - 1);
            shape.translate(pixelPos, (BREADTH - 1) - MAJOR_TICK_HEIGHT + 1);
        }
        else
        {
            shape << QPoint(0,              +(cursorWidth >> 1))
                  << QPoint(0,              -(cursorWidth >> 1))
                  << QPoint(cursorHeight - 1, 0);
            shape.translate((BREADTH - 1) - MAJOR_TICK_HEIGHT + 1, pixelPos);
        }
        painter.drawPolygon(shape);
    }

}
