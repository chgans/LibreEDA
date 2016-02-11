#include "logmodel.h"

#include <QIcon>

LogModel::LogModel(QObject *parent):
    QAbstractTableModel(parent)
{
    m_messageTypeNames.append(QString("Debug"));
    m_messageTypeIconNames.append(QString("flag"));
    m_messageTypeNames.append(QString("Warning"));
    m_messageTypeIconNames.append(QString("flag-yellow"));
    m_messageTypeNames.append(QString("Critical"));
    m_messageTypeIconNames.append(QString("flag-red"));
    m_messageTypeNames.append(QString("Fatal"));
    m_messageTypeIconNames.append(QString("flag-red"));
    m_messageTypeNames.append(QString("Info"));
    m_messageTypeIconNames.append(QString("flag-blue"));
}

LogModel::~LogModel()
{

}

int LogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_messages.count();
}

int LogModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 2;
}


QVariant LogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Q_ASSERT(index.row() < m_messages.count());
    Q_ASSERT(index.column() < 2);

    LogMessage *msg = message(index);

    switch (role)
    {
    case Qt::DisplayRole:
        switch(index.column())
        {
        case 0:
            return QVariant(QString(QByteArray(msg->categoryName)));
        case 1:
            return QVariant(msg->text);
        default:
            return QVariant();
        }
    case Qt::DecorationRole:
        switch(index.column())
        {
        case 0:
            return QVariant(QIcon::fromTheme(m_messageTypeIconNames[msg->messageType]));
        case 1:
            return QVariant();
        default:
            return QVariant();
        }
    default:
        return QVariant();
    }
}

LogMessage *LogModel::message(const QModelIndex &index) const
{
    int row = index.row();

    if (!index.isValid() || row < 0 || row >= m_messages.count())
        return 0;

    return m_messages.value(row);
}

void LogModel::addMessage(LogMessage *message)
{
    int pos = m_messages.count();

    beginInsertRows(QModelIndex(), pos, pos);
    m_messages.append(message);
    endInsertRows();
    if (!m_categories.contains(message->categoryName)) {
        m_categories.append(message->categoryName);
        emit categoryListChanged(m_categories);
    }
}

void LogModel::clearMessages()
{
    beginResetModel();
    qDeleteAll(m_messages);
    m_messages.clear();
    endResetModel();
    if (m_categories.count() > 0) {
        m_categories.clear();
        emit categoryListChanged(m_categories);
    }
}

