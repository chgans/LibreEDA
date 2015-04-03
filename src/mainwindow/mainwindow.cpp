#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphicseditor/graphicseditor.h"
#include "logviewer/logviewer.h"

#include <QDebug>
#include <QDockWidget>
#include <QApplication>

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
    outputPaneDock->setWidget(LogViewer::instance());
    outputPaneDock->setFeatures(QDockWidget::NoDockWidgetFeatures); // not movable, floatable, ...
    outputPaneDock->setTitleBarWidget(new QWidget); // get rid of empty space at the top
    addDockWidget(Qt::BottomDockWidgetArea, outputPaneDock);
}


void MainWindow::focusInEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QMainWindow::focusInEvent(event);
}

void MainWindow::focusOutEvent(QFocusEvent *event)
{
    qDebug() << __PRETTY_FUNCTION__;
    QMainWindow::focusOutEvent(event);
}

bool MainWindow::focusNextPrevChild(bool next)
{
    qDebug() << __PRETTY_FUNCTION__ << "Prev" << QApplication::focusWidget();
    bool found = QMainWindow::focusNextPrevChild(next);
    qDebug() << __PRETTY_FUNCTION__ << "Next" << QApplication::focusWidget();
    return found;
}
