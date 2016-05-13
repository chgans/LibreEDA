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
    explicit LogModel(QObject *parent = nullptr);
    ~LogModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    LogMessage *message(const QModelIndex &index) const;
    //QtMsgType messageType(const QModelIndex &index) const;

    void addMessage(LogMessage *message);

signals:
    void categoryListChanged(const QStringList &categories);

public slots:
    void clearMessages();

private:
    QList<LogMessage*> m_messages;
    QStringList m_messageTypeNames;
    QStringList m_messageTypeIconNames;
    QStringList m_categories;
};

#endif // LOGMODEL_H
