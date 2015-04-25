#ifndef LOGMODEL_H
#define LOGMODEL_H

#include "logviewer_global.h"

#include <QAbstractTableModel>
#include <QList>

#include "logmessage.h"

class LogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    LogModel(QObject *parent = 0);
    ~LogModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    LogMessage *message(const QModelIndex &index) const;
    //QtMsgType messageType(const QModelIndex &index) const;

    void addMessage(LogMessage *message);

signals:
    void categoryListChanged(const QList<const char *> &categories);

public slots:
    void clearMessages();

private:
    QList<LogMessage*> m_messages;
    QList<QString> m_messageTypeNames;
    QList<QString> m_messageTypeIconNames;
    QList<const char *> m_categories;
};

#endif // LOGMODEL_H
