#include "scheditordocument.h"
#include "schscene.h"

#include "item/schitem.h"
#include "item/graphicsbezieritem.h"
#include "item/graphicscircleitem.h"
#include "item/graphicsellipseitem.h"
#include "item/graphicslineitem.h"
#include "item/graphicspolygonitem.h"
#include "item/graphicsrectitem.h"
#include "item/graphicswireitem.h"
#include "item/graphicsbezieritem.h"

#include "xdl/symbol.h"
#include "xdl/symbolwriter.h"
#include "xdl/symbolreader.h"

#include <QFile>
#include <QJsonParseError>
#include <QUndoStack>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(Log)

Q_LOGGING_CATEGORY(Log, "leda.sch.document")

SchEditorDocument::SchEditorDocument(QObject *parent) :
    IDocument(parent),
    m_scene(new SchScene(this)),
    m_commandStack(new QUndoStack(this))
{
    setModified(true);
}

bool SchEditorDocument::load(QString *errorString, const QString &fileName)
{
    xdl::symbol::Reader reader;
    m_xdlSymbol = reader.read(fileName);

    if (m_xdlSymbol == nullptr)
    {
        *errorString = reader.errorString();
        return false;
    }

    return true;
}

QList<SchItem *> SchEditorDocument::items(Qt::SortOrder order) const
{
    Q_UNUSED(order);
    return QList<SchItem *>(); // m_scene->items(order);
}

void SchEditorDocument::executeCommand(SchCommand *command)
{
    Q_UNUSED(command);
    //m_commandStack->push(command);
}

QUndoStack *SchEditorDocument::undoStack()
{
    return m_commandStack;
}


bool SchEditorDocument::save(QString *errorString, const QString &fileName)
{
    xdl::symbol::Writer writer;
    if (!writer.write(fileName, m_xdlSymbol))
    {
        *errorString = writer.errorString();
        return false;
    }
    return true;
}

void SchEditorDocument::render(QPainter *painter)
{
    if (m_scene == nullptr)
    {
        return;
    }

    qCDebug(Log) << QString("Scene rect:") << m_scene->sceneRect()
                 << QString("Painter rect:") << painter->viewport();
    m_scene->render(painter);
}
