#ifndef TASKOPTIONWIDGET_H
#define TASKOPTIONWIDGET_H

#include <QGroupBox>

class QVBoxLayout;

class TaskOptionWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit TaskOptionWidget(QWidget *parent = 0);

    void setCentralWidget(QWidget *widget);

signals:

public slots:

private:
    QVBoxLayout *m_layout;
    QWidget *m_centralWidget;
    QWidget *m_centralPlaceHolderWidget;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
};

#endif // TASKOPTIONWIDGET_H
