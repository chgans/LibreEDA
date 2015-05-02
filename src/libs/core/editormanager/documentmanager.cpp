#include "documentmanager.h"
#include "idocument.h"

#include <QSettings>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QMessageBox>

/*!
  \class DocumentManager
  \preliminary
  \inmodule LibreEDA
  \ingroup LeEditorManager
  \brief The DocumentManager class manages a set of IDocument objects.

  The \c DocumentManager service provides convenience functions for saving and opening
  files:
  \list
   \li \c saveModifiedDocuments() and \c saveModifiedDocumentsSilently(). Both take a list
       of IDocument as an argument, and return the list of files which were _not_ saved.
   \li \c getOpenFileNames(), \c getSaveFileName() and \c getSaveAsFileName() which ask
       the user for file names to open or save.
   \li \c addToRecentFiles() and \c recentFiles() for managing the list of recently opened files.
  \endlist
 */

static const char SETTINGS[] = "DocumentManager";
static const char S_RECENT_FILES[] = "recentFiles";
static const char S_FILE_DIALOG_LAST_DIR[] = "fileDialogLastVisitedDirectory";
static const char S_DEF_LOC_NEW_FILES[] = "defaultLocationForNewFiles";

DocumentManager *DocumentManager::m_instance = nullptr;
QList<QString> DocumentManager::m_recentFiles;
QString DocumentManager::m_fileDialogLastVisitedDirectory;
QString DocumentManager::m_defaultLocationForNewFiles;

/*!
 * Returns the unique instance of the \c DocumentManager.
 */
DocumentManager *DocumentManager::instance()
{
    if (m_instance == nullptr)
        m_instance = new DocumentManager();
    return m_instance;
}

/*!
    Asks the user for a set of file names to be opened. The \a filters
    and \a selectedFilter arguments are interpreted like in
    \c QFileDialog::getOpenFileNames(). \a path specifies a path to open the
    dialog in, if empty then \c DocumentManager::fileDialogLastVisitedDirectory() is used.
*/
QStringList DocumentManager::getOpenFileNames(const QString &filters, const QString &path, QString *selectedFilter)
{
    QString dir = path;
    if (dir.isEmpty()) {
        dir = fileDialogLastVisitedDirectory();
        if (dir.isEmpty())
            dir = "~";
    }
    const QStringList files = QFileDialog::getOpenFileNames(QApplication::activeWindow(),
                                                            tr("Open File"),
                                                            path, filters,
                                                            selectedFilter);
    if (!files.isEmpty())
        setFileDialogLastVisitedDirectory(QFileInfo(files.front()).absolutePath());
    return files;
}

/*!
 * Close the document \a document, asking the user if the document should be saved or not prior closing.
 *
 * Return value:
 * \list
 *  \li \c true if the user accepted to close the document.
 *  \li \c false if the user refused to close the document or if the document couldn't be saved.
 * \endlist
 */
bool DocumentManager::closeDocument(IDocument *document)
{
    if (!document->isModified())
        return true;
    QMessageBox::StandardButton answer;
    answer = QMessageBox::warning(QApplication::activeWindow(),
                                  "Save changes",
                                  QString("Document %1 has unsaved changes, do you want to save it?"),
                                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                  QMessageBox::Cancel);
    if (answer == QMessageBox::Discard)
        return true;
    if (answer == QMessageBox::Cancel)
        return false;

    QString errorString;
    if (document->save(&errorString, document->filePath()))
        return true;
    QString errorMessage = QString("Error while saving %1: %2").arg(document->displayName()).arg(errorString);
    QMessageBox::critical(QApplication::activeWindow(),
                          "Save failed",
                          errorMessage,
                          QMessageBox::Ok);
    return false;
}


/*!
    Adds the \a fileName to the list of recent files.
*/
void DocumentManager::addToRecentFiles(const QString &fileName/*, const QString &editorId*/)
{
    if (m_recentFiles.contains(fileName) || m_recentFiles.count() > 20)
        return;
    m_recentFiles.append(fileName);
}

/*!
    Clears the list of recent files. Should only be called by
    the \LEDA application when the user chooses to clear the list.
*/
void DocumentManager::clearRecentFiles()
{
    m_recentFiles.clear();
}

/*!
    Returns the list of recent files.
*/
QList<QString> DocumentManager::recentFiles()
{
    return m_recentFiles;
}

/*!
  Returns the last visited directory of a file dialog.

  \sa setFileDialogLastVisitedDirectory()
*/
QString DocumentManager::fileDialogLastVisitedDirectory()
{
    return m_fileDialogLastVisitedDirectory;
}

/*!
  Set the last visited directory of a file dialog to \a dir, this will be remembered for the next file dialog.

  \sa fileDialogLastVisitedDirectory()
*/
void DocumentManager::setFileDialogLastVisitedDirectory(const QString &dir)
{
    m_fileDialogLastVisitedDirectory = dir;
}

QString DocumentManager::defaultLocationForNewFiles()
{
    return m_defaultLocationForNewFiles;
}

void DocumentManager::setDefaultLocationForNewFiles(const QString &location)
{
    m_defaultLocationForNewFiles = location;
}

void DocumentManager::saveSettings()
{
    QSettings settings;
    settings.beginGroup(SETTINGS);
    settings.setValue(S_FILE_DIALOG_LAST_DIR, m_fileDialogLastVisitedDirectory);
    settings.setValue(S_DEF_LOC_NEW_FILES, m_defaultLocationForNewFiles);
    settings.beginWriteArray(S_RECENT_FILES, m_recentFiles.count());
    for (int i = 0; i < m_recentFiles.count(); i++) {
        settings.setValue(QString("%1").arg(i), m_recentFiles.at(i));
    }
    settings.endArray();
}

void DocumentManager::loadSettings()
{
    QSettings settings;
    settings.beginGroup(SETTINGS);
    m_fileDialogLastVisitedDirectory = settings.value(S_FILE_DIALOG_LAST_DIR).toString();
    m_defaultLocationForNewFiles = settings.value(S_DEF_LOC_NEW_FILES).toString();
    int nb = settings.beginReadArray(S_RECENT_FILES);
    for (int i = 0; i < nb; i++) {
        m_recentFiles.append(settings.value(QString("%1").arg(i)).toString());
    }
}

DocumentManager::DocumentManager(QObject *parent) :
    QObject(parent)
{
}
