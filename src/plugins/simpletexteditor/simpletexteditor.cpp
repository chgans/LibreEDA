#include "simpletexteditor.h"

#include <QTextEdit>
#include <QTextDocument>

SimpleTextEditor::SimpleTextEditor(QObject *parent) :
    IEditor(parent)
{
    setWidget(new QTextEdit);
}


bool SimpleTextEditor::open(QString *errorString, const QString &fileName)
{
    //m_document = new QTextDocument();
//    m_document->setBaseUrl();
//    QFileInfo fileInfo(fileName);
//    m_document->setDisplayName(fileInfo.baseName());
//    bool result = m_document->load(errorString, m_document->filePath());
//    if (!result)
//        return false;
//    foreach (SchItem *item, m_document->items()) {
//        schWidget()->scene()->addItem(item);
//    }
    return true;
}

IDocument *SimpleTextEditor::document() const
{
    return nullptr;//m_document;
}


QIcon SimpleTextEditor::icon() const
{
    return QIcon(":/icons/sch.png");
}

QString SimpleTextEditor::displayName() const
{
    return "";
}
