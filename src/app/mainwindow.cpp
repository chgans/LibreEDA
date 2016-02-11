#include "mainwindow.h"

#include "core/extension/pluginmanager.h"
#include "core/editormanager/editormanager.h"
#include "core/editormanager/editorview.h"
#include "core/editormanager/ieditor.h"
#include "core/editormanager/idocument.h"
#include "core/editormanager/documentmanager.h"
#include "core/navigationview/navigationdockwidget.h"
#include "core/navigationview/inavigationviewfactory.h"
#include "core/outputpane/outputdockwidget.h"
#include "core/outputpane/ioutputpanefactory.h"

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

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LedaMain)

Q_LOGGING_CATEGORY(LedaMain, "leda.main")

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setWindowTitle(tr("Libre EDA"));

    initEditorView();
    initNavigationView();
    initOutputPane();
    initMenus();
    initActions();
    qCWarning(LedaMain) << "This is a warning message";
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
    IEditor *editortoActivate = nullptr;
    foreach (const QString &fileName, fileNames) {
        IEditor *editor = EditorManager::openEditor(fileName);
        if (editor != nullptr)
            editortoActivate = editor;
    }
    if (editortoActivate)
        m_editorView->setCurrentEditor(editortoActivate);
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
            // TODO: don't bypass MainWindow logic: (see FSNavigator as well)
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
    if (m_currentEditor)
        m_currentEditor->activate(this);
    updateEditorActions();
}

void MainWindow::onEditorCloseRequested(IEditor *editor)
{
    EditorManager::closeEditor(editor);
}

void MainWindow::onEditorAboutToClose(IEditor *editor)
{
    if (editor == m_currentEditor) {
        m_currentEditor->desactivate(this);
        m_currentEditor = nullptr;
    }
    m_editorView->removeEditor(editor);
}

void MainWindow::updateEditorActions()
{
    if (m_currentEditor && m_currentEditor->document()) {
        IDocument *document = m_currentEditor->document();
        QFileInfo fileInfo(document->filePath());
        QString fileName = fileInfo.fileName();
        m_saveAction->setText(QString("&Save \"%1\"").arg(fileName));
        m_saveAction->setEnabled(document->isModified());
        m_saveAsAction->setText(QString("Save \"%1\" &as...").arg(fileName));
        m_saveAsAction->setEnabled(true);
        m_saveAllAction->setEnabled(true);
        m_closeAction->setText(QString("Close \"%1\"").arg(fileName));
        m_closeAction->setEnabled(true);
        m_closeAllExceptAction->setText(QString("Close all except \"%1\"").arg(fileName));
        m_closeAllExceptAction->setEnabled(true);
        m_closeAllAction->setEnabled(true);
    }
    else {
        m_saveAction->setEnabled(false);
        m_saveAsAction->setEnabled(false);
        m_saveAllAction->setEnabled(false);
        m_closeAction->setEnabled(false);
        m_closeAllAction->setEnabled(false);
        m_closeAllExceptAction->setEnabled(false);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    writeSettings();
    EditorManager::saveState();
    DocumentManager::saveSettings();
    event->accept();
}

void MainWindow::initMenus()
{
    m_fileMenu = menuBar()->addMenu("&File");
    m_editMenu = menuBar()->addMenu("&Edit");
    m_windowMenu = menuBar()->addMenu("&Window");
    m_helpMenu = menuBar()->addMenu("&Help");
}

void MainWindow::initActions()
{
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
    m_saveAction = m_fileMenu->addAction(icon, "&Save");
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered,
            this, &MainWindow::onSaveCurrentFileRequested);
    icon = QIcon::fromTheme("document-save-as");
    m_saveAsAction = m_fileMenu->addAction(icon, "&Save as...");
    m_saveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(m_saveAsAction, &QAction::triggered,
            this, &MainWindow::onSaveCurrentFileAsRequested);
    m_saveAllAction = m_fileMenu->addAction("Save &all");
    connect(m_saveAllAction, &QAction::triggered,
            this, &MainWindow::onSaveAllFilesRequested);
    action = m_fileMenu->addAction("Revert to saved"); // TODO: if document modified
    connect(action, &QAction::triggered,
            this, &MainWindow::onRevertCurrentFileRequested);
    action = m_fileMenu->addAction("Reload"); // TODO: if document not modified
    connect(action, &QAction::triggered,
            this, &MainWindow::onReloadCurrentFileRequested);
    m_fileMenu->addSeparator();

    m_closeAction = m_fileMenu->addAction("Close");
    m_closeAction->setShortcut(QKeySequence::Close);
    connect(m_closeAction, &QAction::triggered,
            this, &MainWindow::onCloseCurrentFileRequested);
    m_closeAllExceptAction = m_fileMenu->addAction("Close all except current");
    connect(m_closeAllExceptAction, &QAction::triggered,
            this, &MainWindow::onCloseAllExceptCurrentFileRequested);
    m_closeAllAction = m_fileMenu->addAction("Close all");
    connect(m_closeAllAction, &QAction::triggered,
            this, &MainWindow::onCloseAllFilesRequested);
    m_fileMenu->addSeparator();

    updateEditorActions();

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

void MainWindow::initEditorView()
{
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
            this, &MainWindow::onEditorAboutToClose);
}

void MainWindow::initNavigationView()
{
    m_navigationDockWidget = new NavigationDockWidget;
    m_navigationDockWidget->setFactories(PluginManager::getObjects<INavigationViewFactory>());
    addDockWidget(Qt::LeftDockWidgetArea, m_navigationDockWidget);
}

void MainWindow::initOutputPane()
{
    m_outputPaneDockWidget = new OutputDockWidget;
    m_outputPaneDockWidget->setFactories(PluginManager::getObjects<IOutputPaneFactory>());
    addDockWidget(Qt::BottomDockWidgetArea, m_outputPaneDockWidget);
}
