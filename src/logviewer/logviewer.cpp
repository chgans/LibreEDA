#include "logviewer.h"
#include "logmodel.h"
#include "logwidget.h"
#include "logmessage.h"

#include <QVBoxLayout>

// FIXME
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString text = msg;
    if (msg.startsWith('"'))
        text.remove(0, 1);
    if (msg.endsWith('"'))
        text.remove(text.length()-1, 1);

    LogViewer::instance()->addMessage(new LogMessage(type, text, context.category));
}

LogViewer *LogViewer::m_instance = nullptr;

LogViewer::LogViewer(QWidget *parent):
    QFrame(parent),
    m_widget(new LogWidget),
    m_model(new LogModel(this))
{
    qInstallMessageHandler(myMessageOutput);
    setLayout(new QVBoxLayout);
    layout()->addWidget(m_widget);
    m_widget->setModel(m_model);
}

LogViewer::~LogViewer()
{

}

LogViewer *LogViewer::instance()
{
    if (m_instance == nullptr)
        m_instance = new LogViewer();
    return m_instance;
}

void LogViewer::addMessage(LogMessage *message)
{
    m_model->addMessage(message);
}
