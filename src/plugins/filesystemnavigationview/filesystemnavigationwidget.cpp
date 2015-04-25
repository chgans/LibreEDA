#include "filesystemnavigationwidget.h"
#include "core/editormanager/editormanager.h"
#include "core/editormanager/ieditor.h"
#include <QVBoxLayout>
#include <QListView>
#include <QLabel>
#include <QFileSystemModel>
#include <QApplication>

FileSystemNavigationWidget::FileSystemNavigationWidget(QWidget *parent):
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);
    m_title = new QLabel();
    layout->addWidget(m_title);
    m_dirView = new QListView();
    layout->addWidget(m_dirView);
    m_title->setMargin(5);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setFocusProxy(m_dirView);

    m_fileSystemModel = new QFileSystemModel(this);
    QDir::Filters filters = QDir::Dirs | QDir::NoDot
                            | QDir::Files | QDir::Drives
                            | QDir::Readable| QDir::Writable
                            | QDir::Executable | QDir::Hidden;
    m_fileSystemModel->setFilter(filters);m_dirView->setModel(m_fileSystemModel);
    m_dirView->setIconSize(QSize(16,16));
    m_dirView->setFrameStyle(QFrame::NoFrame);

    connect(m_dirView, &QListView::activated,
            this, &FileSystemNavigationWidget::openItem);

    setCurrentDirectory(BASE_SAMPLE_DIR);
}

FileSystemNavigationWidget::~FileSystemNavigationWidget()
{

}

bool FileSystemNavigationWidget::setCurrentDirectory(const QString &directory)
{
    const QString newDirectory = directory.isEmpty() ? QDir::rootPath() : directory;

    QModelIndex oldRootIndex = m_dirView->rootIndex();
    QModelIndex newRootIndex = m_fileSystemModel->setRootPath(newDirectory);
    if (!newRootIndex.isValid()) {
        setCurrentTitle(QString(), QString());
        return false;
    }
    m_dirView->setRootIndex(newRootIndex);
    const QDir current(QDir::cleanPath(newDirectory));
    setCurrentTitle(current.dirName(),
                    QDir::toNativeSeparators(current.absolutePath()));
    if (oldRootIndex.parent() == newRootIndex) { // cdUp, so select the old directory
        m_dirView->setCurrentIndex(oldRootIndex);
        m_dirView->scrollTo(oldRootIndex, QAbstractItemView::EnsureVisible);
    }

    return !directory.isEmpty();
}

QString FileSystemNavigationWidget::currentDirectory() const
{
    return m_fileSystemModel->rootPath();
}

void FileSystemNavigationWidget::setCurrentTitle(QString dirName, const QString &fullPath)
{
    if (dirName.isEmpty())
        dirName = fullPath;
    m_title->setText(dirName);
    m_title->setToolTip(fullPath);
}

// FOr now, we manage only navigating in folders
void FileSystemNavigationWidget::openItem(const QModelIndex &index)
{
    const QString fileName = m_fileSystemModel->fileName(index);
    if (fileName == QLatin1String("."))
        return;
    if (fileName == QLatin1String("..")) {
        // cd up: Special behaviour: The fileInfo of ".." is that of the parent directory.
        const QString parentPath = m_fileSystemModel->fileInfo(index).absoluteFilePath();
        setCurrentDirectory(parentPath);
        return;
    }

    const QString path = m_fileSystemModel->filePath(index);
    if (m_fileSystemModel->isDir(index)) {
        const QFileInfo fi = m_fileSystemModel->fileInfo(index);
        if (!fi.isReadable() || !fi.isExecutable())
            return;
        // Change to directory
        setCurrentDirectory(path);
        return;
    }

    EditorManager *manager = EditorManager::instance();
    manager->openEditor(path);
}
