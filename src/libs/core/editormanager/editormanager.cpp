#include "editormanager.h"
#include "ieditor.h"
#include "ieditorfactory.h"
#include "idocument.h"
#include "documentmanager.h"

#include <QSettings>
#include <QFileInfo>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

/*!
  \class EditorManager
  \preliminary
  \inmodule LibreEDA
  \ingroup LeEditorManager
  \mainclass
  \brief The EditorManager class manages a set of \c IEditor and \c EditorView objects.

*/

QMap<QString, IEditorFactory *> EditorManager::m_factoryMap;
QMap<QString, IEditor*> EditorManager::m_filePathEditorMap;
IEditor *EditorManager::m_currentEditor = nullptr;
EditorManager *EditorManager::m_instance = nullptr;

EditorManager::EditorManager(QObject *parent) : QObject(parent)
{

}

EditorManager::~EditorManager()
{

}

// TODO:
//  - update recent file list
//  - add editor to open editor list
void EditorManager::addEditor(IEditor *editor, const QString &fileName)
{
    m_filePathEditorMap.insert(fileName, editor);
    emit EditorManager::instance()->editorOpened(editor);
}


EditorManager *EditorManager::instance()
{
    if (m_instance == nullptr)
        m_instance = new EditorManager();
    return m_instance;
}

IEditor *EditorManager::openEditor(const QString &fileName)
{
    QFileInfo fileInfo(fileName);
    QString fileExtension = fileInfo.completeSuffix();
    QString filePath = fileInfo.canonicalFilePath();
    if (!m_factoryMap.contains(fileExtension)) {
        qWarning() << "No factory found for file" << filePath << "with extension" << fileExtension;
        return nullptr;
    }
    if (!fileInfo.exists() || !fileInfo.isReadable() || !fileInfo.isFile()) {
        qWarning() << fileName << "doesn't exists, is not readable or is not a regular file";
        return nullptr;
    }

    if (m_filePathEditorMap.contains(filePath))
        return m_filePathEditorMap.value(filePath);

    IEditorFactory *factory = m_factoryMap.value(fileExtension);
    IEditor *editor = factory->createEditor();
    emit EditorManager::instance()->editorCreated(editor, fileName);

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString errorString;
    if (!editor->open(&errorString, filePath)) {
        QApplication::restoreOverrideCursor();
        if (errorString.isEmpty())
            errorString = tr("Could not open \"%1\": Unknown error").arg(filePath);
        QMessageBox::critical(QApplication::activeWindow(), tr("File Error"), errorString);
        delete editor;
        return nullptr;
    }

    EditorManager::addEditor(editor, filePath);
    QApplication::restoreOverrideCursor();
    return editor;
}

bool EditorManager::closeEditor(IEditor *editor)
{
    if (!DocumentManager::closeDocument(editor->document()))
        return false;
    emit instance()->editorAboutToClose(editor);
    m_filePathEditorMap.remove(editor->document()->filePath());// FIXME
    delete editor;
    emit instance()->editorClosed(editor);
    return true;
}

void EditorManager::saveState()
{
}

bool EditorManager::restoreState()
{
    return false;
}

void EditorManager::registerEditorFactory(IEditorFactory *factory)
{
    if (m_factoryMap.contains(factory->fileExtension())) {
        qWarning() << __PRETTY_FUNCTION__ << "File extension" << factory->fileExtension() << "already registered";
        return;
    }
    m_factoryMap.insert(factory->fileExtension(), factory);
}

IEditorFactory *EditorManager::editorFactory(const QString &fileExtension)
{
    return m_factoryMap.value(fileExtension, nullptr);
}

QStringList EditorManager::supportedFileExtensions()
{
    return m_factoryMap.keys();
}

QString EditorManager::supportedFileFilter()
{
    QStringList filters;
    foreach (const QString &ext, m_factoryMap.keys()) {
        filters.append(QString("*.%1").arg(ext));
    }
    return filters.join(' ');
}
