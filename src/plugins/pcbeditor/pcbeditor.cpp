#include "pcbeditor.h"
#include "pcbdocument.h"
#include "pcbeditorwidget.h"
#include "scene.h"
#include "items/graphicsitem.h"

#include <QFileInfo>

PcbEditor::PcbEditor(QObject *parent):
    IEditor(parent),
    m_document(nullptr)
{
    setWidget(new PcbEditorWidget());
}

PcbEditor::~PcbEditor()
{

}

bool PcbEditor::open(QString *errorString, const QString &fileName)
{
    m_document = new PcbDocument();
    m_document->setFilePath(fileName);
    QFileInfo fileInfo(fileName);
    m_document->setDisplayName(fileInfo.baseName());
    bool result = m_document->load(errorString, m_document->filePath());
    if (!result)
        return false;
    for (GraphicsItem *item: m_document->items()) {
        pcbWidget()->scene()->addItem(item);
    }
    QRectF rect = QRectF(QPointF(0, 0), m_document->boardSize());
    rect.moveTop(-rect.center().y());
    rect.moveLeft(-rect.center().x());
    pcbWidget()->scene()->setSceneRect(rect);
    return true;
}

IDocument *PcbEditor::document() const
{
    return m_document;
}

void PcbEditor::saveState(QSettings *settings) const
{
    Q_UNUSED(settings);
}

bool PcbEditor::restoreState(QSettings *settings)
{
    Q_UNUSED(settings);
    return true;
}

PcbEditorWidget *PcbEditor::pcbWidget() const
{
    return static_cast<PcbEditorWidget *>(widget());
}

QIcon PcbEditor::icon() const
{
    return QIcon(":/icons/pcb.png");
}

QString PcbEditor::displayName() const
{
    return m_document->displayName();
}

void PcbEditor::activate(QMainWindow *mainWindow)
{
    pcbWidget()->activate(mainWindow);
}

void PcbEditor::desactivate(QMainWindow *mainWindow)
{
    pcbWidget()->desactivate(mainWindow);
}
