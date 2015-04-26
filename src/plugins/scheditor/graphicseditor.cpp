#include "graphicseditor.h"
#include "graphicseditorwidget.h"
#include "graphicsdocument.h"

#include <QFileInfo>

GraphicsEditor::GraphicsEditor(QObject *parent) :
    IEditor(parent)
{
    setWidget(new GraphicsEditorWidget());
}


bool GraphicsEditor::open(QString *errorString, const QString &fileName)
{
    m_document = new GraphicsDocument();
    m_document->setFilePath(fileName);
    QFileInfo fileInfo(fileName);
    m_document->setDisplayName(fileInfo.baseName());
    bool result = m_document->load(errorString, m_document->filePath());
    if (!result)
        return false;
    return true;
}

IDocument *GraphicsEditor::document() const
{
    return m_document;
}

GraphicsEditorWidget *GraphicsEditor::schWidget() const
{
    return static_cast<GraphicsEditorWidget *>(widget());
}
