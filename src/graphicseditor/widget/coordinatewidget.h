#ifndef COORDINATEWIDGET_H
#define COORDINATEWIDGET_H

#include <QWidget>

class QDoubleSpinBox;
class QLabel;
class QFocusEvent;

// TODO: rename to CoordinateEditWidget

class CoordinateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CoordinateWidget(QWidget *parent = 0);

    QPointF coordinate() const;

signals:
    void coordinateChanged(const QPointF &point);

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

#endif // COORDINATEWIDGET_H
