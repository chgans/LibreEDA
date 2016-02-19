#include "scheditor.h"
#include "scheditorwidget.h"
#include "scheditordocument.h"
#include "schscene.h"
#include "item/schitem.h"

#include <QFileInfo>

SchEditor::SchEditor(QObject *parent) :
    IEditor(parent)
{
    setWidget(new SchEditorWidget());
}

SchEditor::~SchEditor()
{
    delete m_document;
}


bool SchEditor::open(QString *errorString, const QString &fileName)
{
    m_document = new SchEditorDocument();
    m_document->setFilePath(fileName);
    QFileInfo fileInfo(fileName);
    m_document->setDisplayName(fileInfo.baseName());
    bool result = m_document->load(errorString, m_document->filePath());
    if (!result)
        return false;
    foreach (SchItem *item, m_document->items()) {
        schWidget()->scene()->addItem(item);
    }
    return true;
}

IDocument *SchEditor::document() const
{
    return m_document;
}

SchEditorWidget *SchEditor::schWidget() const
{
    return static_cast<SchEditorWidget *>(widget());
}

QIcon SchEditor::icon() const
{
    return QIcon(":/icons/sch.png");
}

QString SchEditor::displayName() const
{
    return m_document->displayName();
}


void SchEditor::activate(QMainWindow *mainWindow)
{
    schWidget()->activate(mainWindow);
}

void SchEditor::desactivate(QMainWindow *mainWindow)
{
    schWidget()->desactivate(mainWindow);
}
