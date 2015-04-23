#include "editormanager.h"
#include "ieditor.h"
#include "ieditorfactory.h"
#include "idocument.h"

#include <QSettings>
#include <QFileInfo>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

QMap<QString, IEditorFactory *> EditorManager::m_factoryMap;
QMap<QString, IEditor*> EditorManager::m_filePathEditorMap;
QMap<QMainWindow *, EditorView *> EditorManager::m_mainWindowEditorMap;
IEditor *EditorManager::m_currentEditor = nullptr;
EditorManager *EditorManager::m_instance = nullptr;

EditorManager::EditorManager(QObject *parent) : QObject(parent)
{

}

EditorManager::~EditorManager()
{

}

IEditor *EditorManager::createEditor(const QString &fileName)
{
    Q_UNUSED(fileName);
    return nullptr;
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
    EditorManager::activateEditor(editor);
    QApplication::restoreOverrideCursor();
    return editor;
}

void EditorManager::activateEditor(IEditor *editor)
{
    m_currentEditor = editor;
    emit EditorManager::instance()->currentEditorChanged(editor);
}

IEditor *EditorManager::activateEditorForDocument(IDocument *document)
{
    Q_UNUSED(document);
    return nullptr;
}

bool EditorManager::closeDocument(IDocument *document, bool askAboutModifiedEditors)
{
    Q_UNUSED(document);
    Q_UNUSED(askAboutModifiedEditors);
    return false;
}

bool EditorManager::closeDocuments(const QList<IDocument *> &documents, bool askAboutModifiedEditors)
{
    Q_UNUSED(documents);
    Q_UNUSED(askAboutModifiedEditors);
    return false;
}

void EditorManager::closeOtherDocuments(IDocument *document)
{
    Q_UNUSED(document);
    return;
}

bool EditorManager::saveDocument(IDocument *document)
{
    Q_UNUSED(document);
    return false;
}

bool EditorManager::closeEditors(const QList<IEditor *> &editors, bool askAboutModifiedEditors)
{
    Q_UNUSED(editors);
    Q_UNUSED(askAboutModifiedEditors);
    return false;
}

void EditorManager::closeEditor(IEditor *editor, bool askAboutModifiedEditors)
{
    Q_UNUSED(editor);
    Q_UNUSED(askAboutModifiedEditors);
    return;
}

void EditorManager::saveState(QSettings *settings)
{
    Q_UNUSED(settings);
}

bool EditorManager::restoreState(QSettings *settings)
{
    Q_UNUSED(settings);
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

void EditorManager::saveDocument()
{

}

void EditorManager::saveDocumentAs()
{

}

void EditorManager::revertToSaved()
{

}

bool EditorManager::closeAllEditors(bool askAboutModifiedEditors)
{
    Q_UNUSED(askAboutModifiedEditors);
    return false;
}

void EditorManager::slotCloseCurrentDocument()
{

}

void EditorManager::closeOtherDocuments()
{

}
