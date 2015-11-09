#include "scheditordocument.h"
#include "schscene.h"

#include "command/additemcommand.h"

#include "item/schitem.h"
#include "item/graphicsbezieritem.h"
#include "item/graphicscircleitem.h"
#include "item/graphicsellipseitem.h"
#include "item/graphicslineitem.h"
#include "item/graphicspolygonitem.h"
#include "item/graphicsrectitem.h"
#include "item/graphicswireitem.h"
#include "item/graphicsbezieritem.h"

#include "core/json.h"

#include <QFile>
#include <QJsonParseError>
#include <QUndoStack>

SchEditorDocument::SchEditorDocument(QObject *parent) :
    IDocument(parent),
    m_scene(new SchScene(this)),
    m_commandStack(new QUndoStack(this))
{
    setModified(true);
}

bool SchEditorDocument::load(QString *errorString, const QString &fileName)
{
    // Open document
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        *errorString = file.errorString();
        return false;
    }

    // JSON doc parsing and validation
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (doc.isNull()) {
        *errorString = parseError.errorString();
        return false;
    }
    QJsonObject rootObject = doc.object();
    if (rootObject.isEmpty()) {
        *errorString = QString("Document malformed: got an empty root object");
        return false;
    }
    // Doc type check
    QString docType = rootObject.value("leda-document-type").toString();
    if (docType != "sch") {
        *errorString = QString("Unknown document type: \"%1\"").arg(docType);
        return false;
    }

    QJsonValue jsonValue;

    // Parse paper size
    jsonValue = rootObject.value("sheet-size");
    if (jsonValue.isUndefined()) {
        *errorString = QString("Malformed document: missing sheet size");
        return false;
    }
    QSizeF sheetSize;
    if (!Json::toSize(errorString, jsonValue, sheetSize))
        return false;

    // Parse items
    jsonValue = rootObject.value("items");
    if (!jsonValue.isArray()) {
        *errorString = QString("Malformed document: missing item list");
        return false;
    }
    QList<SchItem *> items;
    foreach (QJsonValue value, jsonValue.toArray()) {
        if (!value.isObject()) {
            *errorString = QString("Malformed document: bad item");
            return false;
        }

        QJsonObject jsonObject = value.toObject();
        QString type = jsonObject.value("type").toString();
        SchItem *item;
        if (type == "bezier") {
            item = new GraphicsBezierItem;
        }
        else if (type == "circle") {
            item = new GraphicsCircleItem;
        }
        else if (type == "ellipse") {
            item = new GraphicsEllipseItem;
        }
        else if (type == "line") {
            item = new GraphicsLineItem;
        }
        else if (type == "polygon") {
            item = new GraphicsPolygonItem;
        }
        else if (type == "rectangle") {
            item = new GraphicsRectItem;
        }
        else if (type == "wire") {
            item = new GraphicsWireItem;
        }
        else if (type.isNull()){
            *errorString = QString("Malformed document: missing item type");
            qDeleteAll(items);
            return false;
        }
        else {
            *errorString = QString("Malformed document: \"%1\": unknown item type").arg(type);
            qDeleteAll(items);
            return false;
        }
        if (!item->fromJson(errorString, jsonObject)) {
            qDeleteAll(items);
            return false;
        }
        items.append(item);
    }
    m_items = items;
    return true;
}

QSizeF SchEditorDocument::pageSize() const
{
    return m_scene->sceneRect().size();
}

void SchEditorDocument::setPageSize(const QSizeF &size)
{
    m_scene->setSceneRect(QRectF(QPointF(0, 0), size));
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
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    return true;
}
