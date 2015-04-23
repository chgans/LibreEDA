#include "mainwindow.h"

#include "core/editormanager/editormanager.h"
#include "core/editormanager/editorview.h"
#include "core/editormanager/ieditor.h"
#include "core/editormanager/idocument.h"

#include "core/navigationview/navigationdockwidget.h"
#include "filesystemnavigationview/filesystemnavigationviewfactory.h"

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
    m_editorManager = EditorManager::instance();
    m_editorView = new EditorView();
    setCentralWidget(m_editorView);

    connect(m_editorManager, &EditorManager::editorOpened,
            this, &MainWindow::onEditorOpened);

    m_navigationDockWidget = new NavigationDockWidget;
    QList<INavigationViewFactory *> navigationFactories;
    navigationFactories << new FileSystemNavigationViewFactory();
    m_navigationDockWidget->setFactories(navigationFactories);
    addDockWidget(Qt::LeftDockWidgetArea, m_navigationDockWidget);

    QMenu *menu = menuBar()->addMenu("&file");
    QAction *action = menu->addAction("&open");
    connect(action, &QAction::triggered,
            this, &MainWindow::openFileRequested);
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
    EditorManager *manager = EditorManager::instance();
    QString filter = QString("Libre EDA files (%1)").arg(manager->supportedFileFilter());
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open file",
                                                    BASE_SAMPLE_DIR,
                                                    filter);
    if (fileName.isNull())
        return;
    manager->openEditor(fileName);
}

void MainWindow::onEditorOpened(IEditor *editor)
{
    m_editorView->addEditor(editor);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
//    if (maybeSave()) {
//        writeSettings();
//        event->accept();
//    } else {
//        event->ignore();
//    }
}
