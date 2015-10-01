#include "schdocument.h"
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

SchDocument::SchDocument(QObject *parent) :
    IDocument(parent)
{
    setModified(true);
}

bool SchDocument::load(QString *errorString, const QString &fileName)
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

QList<SchItem *> SchDocument::items() const
{
    return m_items;
}


bool SchDocument::save(QString *errorString, const QString &fileName)
{
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    return true;
}
