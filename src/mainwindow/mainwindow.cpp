#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "core/abstracteditor.h"
#include "pcbeditor/pcbeditorwidget.h"
#include "graphicseditor/graphicseditor.h"
#include "logviewer/logviewer.h"

#include <QDebug>
#include <QDockWidget>
#include <QApplication>
#include <QTableWidget>

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
    ui(new Ui::MainWindow),
    m_activeEditor(nullptr)
{
    ui->setupUi(this);

    m_editorTabWidget = new QTabWidget();
    setCentralWidget(m_editorTabWidget);
    connect(m_editorTabWidget, &QTabWidget::currentChanged,
            this, &MainWindow::activateEditor);

    GraphicsEditor *geditor = new GraphicsEditor();
    m_editorTabWidget->addTab(geditor, "sch");
    PcbEditor *peditor = new PcbEditor();
    m_editorTabWidget->addTab(peditor, "pcb");

    m_editorTabWidget->setCurrentIndex(0);
    m_activeEditor = geditor;
    m_activeEditor->activate(this);

    addLogViewer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addLogViewer()
{
    QDockWidget *outputPaneDock = new QDockWidget();
    outputPaneDock->setWidget(LogViewer::instance());
    outputPaneDock->setFeatures(QDockWidget::NoDockWidgetFeatures); // not movable, floatable, ...
    outputPaneDock->setTitleBarWidget(new QWidget); // get rid of empty space at the top
    addDockWidget(Qt::BottomDockWidgetArea, outputPaneDock);
}

void MainWindow::activateEditor(int tabIndex)
{
    if (m_activeEditor)
        m_activeEditor->desactivate(this);

    m_activeEditor = static_cast<AbstractEditor *>(m_editorTabWidget->widget(tabIndex));

    if (m_activeEditor)
        m_activeEditor->activate(this);
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
