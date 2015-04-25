#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QFrame>

class LogWidget;
class LogModel;
class LogMessage;

class LogViewer: public QFrame
{
    Q_OBJECT

public:
    explicit LogViewer(QWidget *parent = 0);
    ~LogViewer();

    static LogViewer *instance();

    void addMessage(LogMessage *message);

private:
    static LogViewer *m_instance;
    LogWidget *m_widget;
    LogModel *m_model;
};

#endif // LOGVIEWER_H
