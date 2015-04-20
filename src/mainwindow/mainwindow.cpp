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
#include <QSettings>
#include <QCloseEvent>

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

    addLogViewer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// TODO: scan all children for implementaion of IGuiStateSmth
void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("mainwindow");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);

    int currentIndex;
    if (!settings.childGroups().contains("tabs")) {
        qDebug() << "Creating default editors" << settings.childGroups();
        GraphicsEditor *geditor = new GraphicsEditor();
        m_editorTabWidget->addTab(geditor, "sch");
        PcbEditor *peditor = new PcbEditor();
        m_editorTabWidget->addTab(peditor, "pcb");
        m_activeEditor = geditor;
        currentIndex = 0;
    }
    settings.beginGroup("tabs");
    int tabIndex = 0;
    foreach (const QString &key, settings.childGroups()) {
        Q_ASSERT(key.toInt() == tabIndex);
        settings.beginGroup(key);
        const QString &editorType = settings.value("type").toString();
        const QString &docName = settings.value("name").toString();
        AbstractEditor *editor = nullptr;
        if (editorType == "graphicseditor")
            editor = new GraphicsEditor();
        else if (editorType == "pcbeditor")
            editor = new PcbEditor();
        Q_ASSERT(editor != nullptr);
        editor->readSettings(settings);
        m_editorTabWidget->addTab(editor, docName);
        bool active = settings.value("active", false).toBool();
        if (active) {
            currentIndex = tabIndex;
            m_activeEditor = editor;
        }
        settings.endGroup();
        tabIndex++;
    }
    Q_ASSERT(m_activeEditor != nullptr);
    m_editorTabWidget->setCurrentIndex(currentIndex);
    qDebug() << currentIndex << m_activeEditor->type();
    m_activeEditor->activate(this);
    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("mainwindow");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.beginGroup("tabs");
    for (int tabIndex = 0; tabIndex < m_editorTabWidget->count(); tabIndex++) {
        settings.beginGroup(QString("%1").arg(tabIndex));
        AbstractEditor *editor = static_cast<AbstractEditor *>(m_editorTabWidget->widget(tabIndex));
        editor->writeSettings(settings);
        settings.setValue("type", editor->type());
        settings.setValue("name", m_editorTabWidget->tabText(tabIndex));
        settings.setValue("active", m_activeEditor == editor);
        settings.endGroup();
    }
    settings.endGroup();
}

void MainWindow::addLogViewer()
{
    QDockWidget *outputPaneDock = new QDockWidget();
    outputPaneDock->setWidget(LogViewer::instance());
    outputPaneDock->setFeatures(QDockWidget::NoDockWidgetFeatures); // not movable, floatable, ...
    outputPaneDock->setTitleBarWidget(new QWidget); // get rid of empty space at the top
    addDockWidget(Qt::BottomDockWidgetArea, outputPaneDock);
}

bool MainWindow::maybeSave()
{
    return true;
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


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
