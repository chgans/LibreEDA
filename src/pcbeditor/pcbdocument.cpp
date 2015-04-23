#include "pcbdocument.h"
#include "designlayer.h"
#include "items/graphicsline.h"
#include "items/graphicsrect.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

PcbDocument::PcbDocument(QObject *parent):
    IDocument(parent)
{

}

PcbDocument::~PcbDocument()
{

}

// TODO: use QJson line/column number in error report
bool PcbDocument::load(QString *errorString, const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        *errorString = file.errorString();
        return false;
    }
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
    QString docType = rootObject.value("leda-document-type").toString();
    if (docType != "pcb") {
        *errorString = QString("Unknown document type: \"%1\"").arg(docType);
        return false;
    }
    QJsonValue jsonLayersValue = rootObject.value("layers");
    QJsonValue jsonItemsValue = rootObject.value("items");
    if (!jsonLayersValue.isArray() || !jsonItemsValue.isArray()) {
        *errorString = QString("Malformed document: missing items and/or layer arrays");
        return false;
    }

    // Parse layers
    QList<int> layerIndexes;
    foreach (QJsonValue value, jsonLayersValue.toArray()) {
        if (!value.isDouble()) {
            // TODO: auto delete layer objects
            *errorString = QString("Malformed document: bad layer index");
            return false;
        }
        int index = value.toInt(-1);
        if (index < 0 || index > 105) {
            *errorString = QString("Malformed document: invalid layer index");
            return false;
        }
        layerIndexes.append(index);
    }

    // Parse items
    QList<GraphicsItem *> items;
    foreach (QJsonValue value, jsonItemsValue.toArray()) {
        if (!value.isObject()) {
            *errorString = QString("Malformed document: bad item");
            return false;
        }
        QJsonObject jsonObject = value.toObject();
        QJsonValue jsonType = jsonObject.value("type");
        QJsonValue jsonPosition = jsonObject.value("position");
        QJsonValue jsonLayer = jsonObject.value("layer");
        if (!jsonType.isString() || !jsonPosition.isArray() || !jsonLayer.isDouble()) {
            *errorString = QString("Malformed document: bad item");
            return false;
        }
        QString type = jsonType.toString();
        int layerIndex = jsonLayer.toInt(-1);
        if (!layerIndexes.contains(layerIndex)) {
            *errorString = QString("Malformed document: item is on an unknown layer");
            return false;
        }
        if (jsonPosition.toArray().count() != 3) {
            *errorString = QString("Malformed document: item position");
            return false;
        }
        qreal pos[3];
        QJsonArray jsonArray = jsonPosition.toArray();
        for (int i = 0; i < 3; i++) {
            if (!jsonArray.at(i).isDouble()) {
                *errorString = QString("Malformed document: item position");
                return false;
            }
            pos[i] = jsonArray.at(i).toDouble();
        }
        GraphicsItem *item;
        if (type == "rectangle") {
            GraphicsRect *rectItem = new GraphicsRect();
            item = rectItem;
        }
        else if (type == "polyline") {
            GraphicsLine *lineItem = new GraphicsLine();
            item = lineItem;
        }

        item->setPos(pos[0], pos[1]);
        item->setZValue(pos[2]);
    }

    m_items = items;
    //m_layers = layers;
    return true;
}

bool PcbDocument::save(QString *errorString, const QString &fileName)
{
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    return false;
}
