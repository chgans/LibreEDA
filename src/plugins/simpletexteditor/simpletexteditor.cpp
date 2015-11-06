#include "simpletexteditor.h"
#include "simpletextdocument.h"

#include <QTextEdit>
#include <QFileInfo>

SimpleTextEditor::SimpleTextEditor(QObject *parent) :
    IEditor(parent),
    m_textEdit(new QTextEdit)
{
    setWidget(m_textEdit);
}


bool SimpleTextEditor::open(QString *errorString, const QString &fileName)
{
    m_document = new SimpleTextDocument();
    m_document->setFilePath(fileName);
    QFileInfo fileInfo(fileName);
    m_document->setDisplayName(fileInfo.baseName());
    bool result = m_document->load(errorString, m_document->filePath());
    if (!result)
        return false;
    m_textEdit->setDocument(m_document->textDocument());
    return true;
}

IDocument *SimpleTextEditor::document() const
{
    return m_document;
}


QIcon SimpleTextEditor::icon() const
{
    return QIcon(":/icons/text-document-icon.png");
}

QString SimpleTextEditor::displayName() const
{
    return m_document->displayName();
}
