#ifndef COORDINATEWIDGET_H
#define COORDINATEWIDGET_H

#include <QWidget>

class QDoubleSpinBox;
class QLabel;
class QFocusEvent;

// TODO: rename to CoordinateEditWidget
//       or CoordinateSpinBox
//       or CartesianCoordinate...

class CoordinateWidget : public QWidget
{
    Q_OBJECT

public:
    // TODO: add enableAxis
    enum Axis {
        XAxis = 0,
        YAxis,
        ZAxis
    };
    Q_DECLARE_FLAGS(Axes, Axis)

    explicit CoordinateWidget(QWidget *parent = 0);

    QPointF coordinate() const;

signals:
    void coordinateChanged(Axis axis, qreal value);
    void coordinateEditingFinished();

public slots:
    void setCoordinate(const QPointF &point);

private:
    QPointF m_point;
    QLabel *m_xLabel;
    QDoubleSpinBox *m_xSpinBox;
    QLabel *m_yLabel;
    QDoubleSpinBox *m_ySpinBox;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

    // QObject interface
public:
    bool eventFilter(QObject *object, QEvent *event);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CoordinateWidget::Axes)

#endif // COORDINATEWIDGET_H
