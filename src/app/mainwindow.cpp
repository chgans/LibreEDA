#include "mainwindow.h"

#include "core/extension/pluginmanager.h"
#include "core/editormanager/editormanager.h"
#include "core/editormanager/editorview.h"
#include "core/editormanager/ieditor.h"
#include "core/editormanager/idocument.h"
#include "core/editormanager/documentmanager.h"
#include "core/navigationview/navigationdockwidget.h"
#include "core/navigationview/inavigationviewfactory.h"

#include <QDebug>
#include <QDockWidget>
#include <QApplication>
#include <QSettings>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setWindowTitle(tr("Libre EDA"));

    // initEditorView()
    EditorManager::initialise();
    m_editorView = new EditorView();
    setCentralWidget(m_editorView);
    connect(m_editorView, &EditorView::editorCloseRequested,
            this, &MainWindow::onEditorCloseRequested);
    connect(m_editorView, &EditorView::currentEditorChanged,
            this, &MainWindow::onCurrentEditorChanged);
    connect(EditorManager::instance(), &EditorManager::editorOpened,
            this, &MainWindow::onEditorOpened);
    connect(EditorManager::instance(), &EditorManager::editorAboutToClose,
            this, &MainWindow::onEditorCloseRequested);

    // initNavigationView()
    m_navigationDockWidget = new NavigationDockWidget;
    m_navigationDockWidget->setFactories(PluginManager::getObjects<INavigationViewFactory>());
    addDockWidget(Qt::LeftDockWidgetArea, m_navigationDockWidget);

    // initOutputPane()
    /* TODO */

    // initMenus()
    m_fileMenu = menuBar()->addMenu("&File");
    m_editMenu = menuBar()->addMenu("&Edit");
    m_windowMenu = menuBar()->addMenu("&Window");
    m_helpMenu = menuBar()->addMenu("&Help");

    // Other, eg initActionManager()
    QAction *action;
    QIcon icon;
    icon = QIcon::fromTheme("document-new");
    action = m_fileMenu->addAction(icon, "&New...");
    action->setShortcut(QKeySequence::New);
    connect(action, &QAction::triggered,
            this, &MainWindow::onNewFileRequested);
    icon = QIcon::fromTheme("document-open");
    action = m_fileMenu->addAction(icon, "&Open...");
    action->setShortcut(QKeySequence::Open);
    connect(action, &QAction::triggered,
            this, &MainWindow::onOpenFileRequested);
    m_recentFilesMenu = m_fileMenu->addMenu("Recent &Files");
    m_fileMenu->addSeparator();

    connect(m_recentFilesMenu, &QMenu::aboutToShow,
            this, &MainWindow::onRecentFilesRequested);
    icon = QIcon::fromTheme("document-save");
    action = m_fileMenu->addAction(icon, "&Save");
    action->setShortcut(QKeySequence::Save);
    connect(action, &QAction::triggered,
            this, &MainWindow::onSaveCurrentFileRequested);
    icon = QIcon::fromTheme("document-save-as");
    action = m_fileMenu->addAction(icon, "&Save as...");
    action->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(action, &QAction::triggered,
            this, &MainWindow::onSaveCurrentFileAsRequested);
    action = m_fileMenu->addAction("Save &all");
    connect(action, &QAction::triggered,
            this, &MainWindow::onSaveAllFilesRequested);
    action = m_fileMenu->addAction("Revert to saved"); // TODO: if document modified
    connect(action, &QAction::triggered,
            this, &MainWindow::onRevertCurrentFileRequested);
    action = m_fileMenu->addAction("Reload"); // TODO: if document not modified
    connect(action, &QAction::triggered,
            this, &MainWindow::onReloadCurrentFileRequested);
    m_fileMenu->addSeparator();

    action = m_fileMenu->addAction("Close");
    action->setShortcut(QKeySequence::Close);
    connect(action, &QAction::triggered,
            this, &MainWindow::onCloseCurrentFileRequested);
    action = m_fileMenu->addAction("Close all except current");
    connect(action, &QAction::triggered,
            this, &MainWindow::onCloseAllExceptCurrentFileRequested);
    action = m_fileMenu->addAction("Close all");
    connect(action, &QAction::triggered,
            this, &MainWindow::onCloseAllFilesRequested);
    m_fileMenu->addSeparator();

    icon = QIcon::fromTheme("document-print");
    action = m_fileMenu->addAction(icon, "&Print...");
    action->setShortcut(QKeySequence::Print);
    connect(action, &QAction::triggered,
            this, &MainWindow::onPrintCurrentFileRequested);
    m_fileMenu->addSeparator();

    icon = QIcon::fromTheme("application-exit");
    action = m_fileMenu->addAction(icon, "E&xit");
    action->setShortcut(QKeySequence::Quit);
    connect(action, &QAction::triggered,
            this, &MainWindow::onApplicationExitRequested);
}

MainWindow::~MainWindow()
{
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("mainwindow");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("mainwindow");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

void MainWindow::onNewFileRequested()
{

}

void MainWindow::onOpenFileRequested()
{
    qDebug() << EditorManager::supportedFileFilter();
    QString filter = QString("Libre EDA files (%1)").arg(EditorManager::supportedFileFilter());
    QStringList fileNames = DocumentManager::getOpenFileNames(filter, BASE_SAMPLE_DIR);
    if (fileNames.isEmpty())
        return;
    foreach (const QString &fileName, fileNames)
        EditorManager::openEditor(fileName);

}

void MainWindow::onRecentFilesRequested()
{
    m_recentFilesMenu->clear();
    if (DocumentManager::recentFiles().isEmpty())
        return;
    QAction *action;
    foreach (const QString &file, DocumentManager::recentFiles()) {
        action = m_recentFilesMenu->addAction(file);
        connect(action, &QAction::triggered,
                this, [this, file](bool) {
           EditorManager::openEditor(file);
        });
    }
    m_recentFilesMenu->addSeparator();
    action = m_recentFilesMenu->addAction("&Clear");
    connect(action, &QAction::triggered,
            this, [this](bool) {
         DocumentManager::clearRecentFiles();
    });
}

void MainWindow::onSaveCurrentFileRequested()
{
    IDocument *document = m_editorView->currentEditor()->document();
    DocumentManager::saveDocument(document, document->filePath());
}

void MainWindow::onSaveCurrentFileAsRequested()
{
    if (m_editorView->editorCount() == 0)
        return;
    IDocument *document = m_editorView->currentEditor()->document();
    Q_ASSERT(document != nullptr);

    QString fileName = DocumentManager::getSaveAsFileName(document);
    DocumentManager::saveDocument(document, fileName);
}

void MainWindow::onSaveAllFilesRequested()
{
    //DocumentManager::saveAllModifiedDocumentsSilently();
}

void MainWindow::onRevertCurrentFileRequested()
{

}

void MainWindow::onReloadCurrentFileRequested()
{

}

void MainWindow::onCloseCurrentFileRequested()
{

}

void MainWindow::onCloseAllExceptCurrentFileRequested()
{

}

void MainWindow::onCloseAllFilesRequested()
{

}

void MainWindow::onPrintCurrentFileRequested()
{

}

void MainWindow::onApplicationExitRequested()
{

}

void MainWindow::onEditorOpened(IEditor *editor)
{
    m_editorView->addEditor(editor);
    m_editorView->setCurrentEditor(editor);
    DocumentManager::addToRecentFiles(editor->document()->filePath());
}

void MainWindow::onCurrentEditorChanged(IEditor *editor)
{
    if (m_currentEditor)
        m_currentEditor->desactivate(this);
    m_currentEditor = editor;
    m_currentEditor->activate(this);
}

void MainWindow::onEditorCloseRequested(IEditor *editor)
{
    EditorManager::closeEditor(editor);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    writeSettings();
    EditorManager::saveState();
    DocumentManager::saveSettings();
    event->accept();
}
