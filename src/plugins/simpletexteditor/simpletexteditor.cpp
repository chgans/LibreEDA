#include "simpletexteditor.h"
#include "simpletextdocument.h"

#include <QTextEdit>

SimpleTextEditor::SimpleTextEditor(QObject *parent) :
    IEditor(parent),
    m_textEdit(new QTextEdit)
{
    setWidget(m_textEdit);
}


bool SimpleTextEditor::open(QString *errorString, const QString &fileName)
{
    m_document = new SimpleTextDocument();
    m_document->load(errorString, fileName);
    m_textEdit->setDocument(m_document->textDocument());
//    m_document->setBaseUrl();
//    QFileInfo fileInfo(fileName);
//    m_document->setDisplayName(fileInfo.baseName());
//    bool result = m_document->load(errorString, m_document->filePath());
//    if (!result)
//        return false;
    return true;
}

IDocument *SimpleTextEditor::document() const
{
    return m_document;
}


QIcon SimpleTextEditor::icon() const
{
    return QIcon(":/icons/sch.png");
}

QString SimpleTextEditor::displayName() const
{
    return "";
}
