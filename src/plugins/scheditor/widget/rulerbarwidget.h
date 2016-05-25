#ifndef GRAPHICSVIEWRULER_H
#define GRAPHICSVIEWRULER_H

#include <QWidget>

namespace SymbolEditor
{

    class RulerBarWidget : public QWidget
    {
        Q_OBJECT
        Q_ENUMS(Alignment)

    public:
        enum Alignment { Horizontal, Vertical };
        static const int BREADTH;

        explicit RulerBarWidget(RulerBarWidget::Alignment alignment, QWidget *parent = nullptr);
        ~RulerBarWidget();

        void setCursorRange(qreal first, qreal last);
        void setCursorPosition(const QPointF &pos);

        QSize minimumSizeHint() const;
        RulerBarWidget::Alignment rulerType() const;

        void setBackgroundColor(const QColor &color);
        QColor backgroundColor() const;

        void setForegroundColor(const QColor &color);
        QColor foregroundColor() const;


    protected:
        void paintEvent(QPaintEvent *event);

    private:
        Alignment m_alignment;
        QColor m_backgroundColor;
        QColor m_foregroundColor;
        qreal m_firstPos;
        qreal m_lastPos;
        qreal m_currentPos;
        int length();

        void drawMinorTick(QPainter &painter, int pixelPos);
        void drawMajorTick(QPainter &painter, int pixelPos, qreal logicalPos);
        void drawIndicator(QPainter &painter, int pixelPos);
    };

}

#endif // GRAPHICSVIEWRULER_H
