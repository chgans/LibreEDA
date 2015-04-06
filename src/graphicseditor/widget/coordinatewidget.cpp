#include "coordinatewidget.h"

#include <QDebug>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QFormLayout>
#include <QFocusEvent>

CoordinateWidget::CoordinateWidget(QWidget *parent) :
    QWidget(parent)
{
    QFormLayout *layout = new QFormLayout();
    setLayout(layout);

    void (QDoubleSpinBox:: *signalValueChanged)(double);
    signalValueChanged = &QDoubleSpinBox::valueChanged;

    m_xLabel = new QLabel("Global X");
    m_xSpinBox = new QDoubleSpinBox();
    m_xSpinBox->setSuffix("  mm");
    layout->addRow(m_xLabel, m_xSpinBox);
    connect(m_xSpinBox, signalValueChanged,
            [this](double value) {
        if (m_point.x() == value)
            return;
        m_point.setX(value);
        emit coordinateChanged(XAxis, value);
    });

    m_yLabel = new QLabel("Global Y");
    m_ySpinBox = new QDoubleSpinBox();
    m_ySpinBox->setSuffix("  mm");
    layout->addRow(m_yLabel, m_ySpinBox);
    connect(m_ySpinBox, signalValueChanged,
            [this](double value) {
        if (m_point.y() == value)
            return;
        m_point.setY(value);
        emit coordinateChanged(YAxis, value);
    });

    m_xSpinBox->installEventFilter(this);
    m_ySpinBox->installEventFilter(this);
}

QPointF CoordinateWidget::coordinate() const
{
    return m_point;
}

void CoordinateWidget::setCoordinate(const QPointF &point)
{
    m_point = point;
    m_xSpinBox->setValue(point.x());
    m_ySpinBox->setValue(point.y());
}

void CoordinateWidget::focusInEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QWidget::focusInEvent(event);
    m_xSpinBox->setFocus(event->reason());
}

void CoordinateWidget::focusOutEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QWidget::focusOutEvent(event);
}

bool CoordinateWidget::eventFilter(QObject *object, QEvent *event)
{
    if ((object != m_xSpinBox && object != m_ySpinBox))
        return QWidget::eventFilter(object, event);

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *kevent = static_cast<QKeyEvent*>(event);
        if (kevent->key() == Qt::Key_Enter ||
                kevent->key() == Qt::Key_Return) {
            focusNextChild();
            if (object == m_ySpinBox)
                emit coordinateEditingFinished();
            return true;
        }
#if 0 // Attempt to let single key stroke shortcuts go through
        else if (kevent->text().contains(QRegularExpression("^[[:alpha:]]+$"))) {
            qDebug() << "seen" << kevent->text();
            QKeyEvent *fEvent = new QKeyEvent(QEvent::KeyPress, kevent->key(), kevent->modifiers());
            qApp->sendEvent(qApp->activeWindow(), fEvent);
            return false;
        }
#endif
    }
    return QWidget::eventFilter(object, event);
}
