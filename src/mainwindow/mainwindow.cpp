#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphicseditor/graphicseditor.h"
#include "logviewer/logviewer.h"

#include <QDockWidget>


// Menus
//  - file
//  - edit
//  - place
//  - tools
//  - view
//  - help

// TODO: message format and handler
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    addGraphicsEditor();
    addLogViewer();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addGraphicsEditor()
{
    GraphicsEditor *editor = new GraphicsEditor();
    setCentralWidget(editor);
    editor->activate(this);
}

void MainWindow::addLogViewer()
{
    QDockWidget *outputPaneDock = new QDockWidget();
    outputPaneDock->setWidget(new LogViewer());
    outputPaneDock->setFeatures(QDockWidget::NoDockWidgetFeatures); // not movable, floatable, ...
    outputPaneDock->setTitleBarWidget(new QWidget); // get rid of empty space at the top
    addDockWidget(Qt::BottomDockWidgetArea, outputPaneDock);
}
