#ifndef GRAPHICSVIEWRULER_H
#define GRAPHICSVIEWRULER_H

#include <QWidget>

class GraphicsViewRuler : public QWidget
{
    Q_OBJECT
    Q_ENUMS(Alignment)

public:
    enum Alignment { Horizontal, Vertical };
    static const int BREADTH;

    explicit GraphicsViewRuler(GraphicsViewRuler::Alignment alignment, QWidget *parent = 0);
    ~GraphicsViewRuler();

    void setCursorRange(qreal first, qreal last);
    void setCursorPosition(const QPointF &pos);

    QSize minimumSizeHint() const;
    GraphicsViewRuler::Alignment rulerType() const;

protected:
    void paintEvent(QPaintEvent* event);

private:
    Alignment m_alignment;
    qreal m_firstPos;
    qreal m_lastPos;
    qreal m_currentPos;
    int length();

    void drawMinorTick(QPainter &painter, int pixelPos);
    void drawMajorTick(QPainter &painter, int pixelPos, qreal logicalPos);
    void drawIndicator(QPainter &painter, int pixelPos);
};
#endif // GRAPHICSVIEWRULER_H
