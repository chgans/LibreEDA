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

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    bool focusNextPrevChild(bool next);
};

#endif // TASKOPTIONWIDGET_H
