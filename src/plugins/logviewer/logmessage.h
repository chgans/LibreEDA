#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include "logviewer_global.h"

#include <QtGlobal>
#include <QString>


class LogMessage
{
public:
    LogMessage();
    LogMessage(QtMsgType messageType, const QString &text, const QString &categoryName = QString());
    ~LogMessage();

    QtMsgType messageType;
    QString text;
    QString categoryName;

};
/*
bool operator==(const LogMessage &t1, const LogMessage &t2);
uint qHash(const LogMessage &LogMessage);
bool operator<(const LogMessage &a, const LogMessage &b);
*/
#endif // LOGMESSAGE_H
