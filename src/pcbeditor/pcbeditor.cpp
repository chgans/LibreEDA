#include "pcbeditor.h"
#include "pcbdocument.h"
#include "pcbeditorwidget.h"
#include <QFileInfo>

PcbEditor::PcbEditor(QObject *parent):
    IEditor(parent)
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
    *errorString = "";
    return true;
}

IDocument *PcbEditor::document()
{
    return m_document;
}

void PcbEditor::saveState(QSettings *settings) const
{
    pcbWidget()->writeSettings(*settings);
}

bool PcbEditor::restoreState(QSettings *settings)
{
    pcbWidget()->readSettings(*settings);
    return true;
}

PcbEditorWidget *PcbEditor::pcbWidget() const
{
    return static_cast<PcbEditorWidget *>(widget());
}
