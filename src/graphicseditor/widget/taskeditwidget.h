#ifndef TASKEDITWIDGET_H
#define TASKEDITWIDGET_H

#include <QGroupBox>

class QPushButton;
class QVBoxLayout;

class TaskEditWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit TaskEditWidget(QWidget *parent = 0);

    void setCentralWidget(QWidget *widget);

signals:

public slots:

private:
    QVBoxLayout *m_layout;
    QPushButton *m_cancelButton;
    QWidget *m_centralWidget;
    QWidget *findLastChildToFocus(QObject *parent);

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    bool focusNextPrevChild(bool next);
};

#endif // TASKEDITWIDGET_H
