#include "ieditorfactory.h"

/*!
  \class IEditorFactory
  \preliminary
  \brief The IEditorFactory class ...
  */

IEditorFactory::IEditorFactory(QObject *parent) : QObject(parent)
{

}

IEditorFactory::~IEditorFactory()
{

}

QString IEditorFactory::displayName() const
{
    return m_displayName;
}

void IEditorFactory::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}

QString IEditorFactory::id() const
{
    return m_id;
}

void IEditorFactory::setId(const QString &id)
{
    m_id = id;
}

QString IEditorFactory::fileExtension() const
{
    return m_fileExtension;
}

void IEditorFactory::setFileExtension(const QString &fileExtension)
{
    m_fileExtension = fileExtension;
}


