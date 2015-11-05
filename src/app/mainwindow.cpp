#include "mainwindow.h"

#include "core/editormanager/editormanager.h"
#include "core/editormanager/editorview.h"
#include "core/editormanager/ieditor.h"
#include "core/editormanager/idocument.h"
#include "core/editormanager/documentmanager.h"
#include "core/navigationview/navigationdockwidget.h"
#include "core/extension/pluginmanager.h"

#include <QDebug>
#include <QDockWidget>
#include <QApplication>
#include <QSettings>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    EditorManager::initialise();

    m_editorView = new EditorView();
    setCentralWidget(m_editorView);

//    connect(m_editorView, &EditorView::editorCloseRequested,
//            this, &MainWindow::onEditorCloseRequested);
//    connect(EditorManager::instance(), &EditorManager::editorOpened,
//            this, &MainWindow::onEditorOpened);
//    connect(EditorManager::instance(), &EditorManager::editorAboutToClose,
//            m_editorView, &EditorView::removeEditor);

    m_navigationDockWidget = new NavigationDockWidget;
    m_navigationDockWidget->setFactories(PluginManager::getObjects<INavigationViewFactory>());
    addDockWidget(Qt::LeftDockWidgetArea, m_navigationDockWidget);

    QAction *action;
    QMenu *fileMenu = menuBar()->addMenu("&file");
    action = fileMenu->addAction("&open");
    connect(action, &QAction::triggered,
            this, &MainWindow::openFileRequested);
    m_recentFilesMenu = fileMenu->addMenu("Recent &Files");
    connect(m_recentFilesMenu, &QMenu::aboutToShow,
            this, &MainWindow::populateRecentFilesMenu);
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

void MainWindow::openFileRequested(bool)
{
    QString filter = QString("Libre EDA files (%1)").arg(EditorManager::supportedFileFilter());
    QStringList fileNames = DocumentManager::getOpenFileNames(filter, BASE_SAMPLE_DIR);
    foreach (const QString &fileName, fileNames)
        EditorManager::openEditor(fileName);
}

void MainWindow::populateRecentFilesMenu()
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

void MainWindow::onEditorOpened(IEditor *editor)
{
    m_editorView->addEditor(editor);
    DocumentManager::addToRecentFiles(editor->document()->filePath());
}

void MainWindow::onEditorCloseRequested(IEditor *editor)
{
    EditorManager::closeEditor(editor);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    EditorManager::saveState();
    DocumentManager::saveSettings();
    event->accept();
}
