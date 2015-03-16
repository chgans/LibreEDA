#include "logsortfilterproxymodel.h"
#include "logmodel.h"

LogSortFilterProxyModel::LogSortFilterProxyModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    m_messageType[QtDebugMsg] = true;
    m_messageType[QtWarningMsg] = true;
    m_messageType[QtCriticalMsg] = true;
    m_messageType[QtFatalMsg] = true;
}

LogSortFilterProxyModel::~LogSortFilterProxyModel()
{

}

bool LogSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    LogModel *model = static_cast<LogModel *>(sourceModel());
    QModelIndex index = model->index(source_row, 0, source_parent);
    LogMessage *msg = model->message(index);
    QtMsgType type = msg->messageType;
    Q_ASSERT(type >= 0 && type < 4);
    const char *category = msg->categoryName;
    return filterIncludesMessageType(type) &&
            filterIncludesCategoryName(category) &&
            QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool LogSortFilterProxyModel::filterIncludesCategoryName(const char *categoryName) const
{
    if (!m_category.contains(categoryName))
        return true;
    return m_category.value(categoryName);
}

bool LogSortFilterProxyModel::filterIncludesMessageType(QtMsgType messageType) const
{
    return m_messageType.value(messageType, false);
}

void LogSortFilterProxyModel::setFilterIncludesMessageType(QtMsgType messageType, bool includes)
{
    Q_ASSERT(messageType >= 0 && messageType < 4);
    m_messageType[messageType] = includes;
    invalidateFilter();
}

void LogSortFilterProxyModel::setFilterIncludesCategoryName(const char *categoryName, bool includes)
{
    m_category[categoryName] = includes;
    invalidateFilter();
}

bool LogSortFilterProxyModel::filterIncludesDebugMessages() const
{
    return filterIncludesMessageType(QtDebugMsg);
}

bool LogSortFilterProxyModel::filterIncludesWarningMessages() const
{
    return filterIncludesMessageType(QtWarningMsg);
}

bool LogSortFilterProxyModel::filterIncludesCriticalMessages() const
{
    return filterIncludesMessageType(QtCriticalMsg);
}

bool LogSortFilterProxyModel::filterIncludesFatalMessages() const
{
    return filterIncludesMessageType(QtFatalMsg);
}

void LogSortFilterProxyModel::setFilterIncludesDebugMessages(bool includes)
{
    setFilterIncludesMessageType(QtDebugMsg, includes);
}

void LogSortFilterProxyModel::setFilterIncludesWarningMessages(bool includes)
{
    setFilterIncludesMessageType(QtWarningMsg, includes);
}

void LogSortFilterProxyModel::setFilterIncludesCriticalMessages(bool includes)
{
    setFilterIncludesMessageType(QtCriticalMsg, includes);
}

void LogSortFilterProxyModel::setFilterIncludesFatalMessages(bool includes)
{
    setFilterIncludesMessageType(QtFatalMsg, includes);
}

