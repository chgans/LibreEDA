#include "idocument.h"

/*!
  \class IDocument
  \preliminary
  \inmodule LibreEDA
  \ingroup LeEditorManager
  \mainclass
  \brief The IDocument class ...
*/

IDocument::IDocument(QObject *parent) : QObject(parent)
{
    setModified(false);
}

IDocument::~IDocument()
{

}

void IDocument::setId(const QString &id)
{
    m_id = id;
}

QString IDocument::id() const
{
    return m_id;
}

const QString IDocument::filePath() const
{
    return m_filePath;
}

void IDocument::setFilePath(const QString &filePath)
{
    if (filePath == m_filePath)
        return;
    QString old = m_filePath;
    m_filePath = filePath;
    emit filePathChanged(old, m_filePath);
}

QString IDocument::displayName() const
{
    return m_displayName;
}

void IDocument::setDisplayName(const QString &name)
{
    if (name == m_displayName)
        return;
    m_displayName = name;
    emit displayNameChanged(m_displayName);
}

bool IDocument::isModified() const
{
    return m_modified;
}

void IDocument::setModified(bool modified)
{
    if (modified == m_modified)
        return;
    m_modified = modified;
    emit modifiedChanged(m_modified);
}

