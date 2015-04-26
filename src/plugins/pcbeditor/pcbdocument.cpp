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
    // Open document
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        *errorString = file.errorString();
        return false;
    }

    // JSON parsing and doc-type check
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

    // Parse board size
    QJsonValue jsonSizeValue = rootObject.value("size");
    if (!jsonSizeValue.isArray() || jsonSizeValue.toArray().count() != 2) {
        *errorString = QString("Malformed document: missing board size or bad value");
        return false;
    }
    qreal size[2];
    QSizeF boardSize;
    QJsonArray jsonSize = jsonSizeValue.toArray();
    for (int i = 0; i < 2; i++) {
        if (!jsonSize.at(i).isDouble()) {
            *errorString = QString("Malformed document: bad size value");
            return false;
        }
        size[i] = jsonSize.at(i).toDouble();
    }
    boardSize = QSizeF(size[0], size[1]);

    // Parse layers
    QJsonValue jsonLayersValue = rootObject.value("layers");
    if (!jsonLayersValue.isArray()) {
        *errorString = QString("Malformed document: missing layer list");
        return false;
    }
    QList<int> layers;
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
        layers.append(index);
    }

    // Parse items
    QJsonValue jsonItemsValue = rootObject.value("items");
    if (!jsonItemsValue.isArray()) {
        *errorString = QString("Malformed document: missing item list");
        return false;
    }
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
        if (!layers.contains(layerIndex)) {
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
            QJsonValue jsonLine = jsonObject.value("points");
            if (!jsonLine.isArray() || jsonLine.toArray().count() != 2) {
                *errorString = QString("Malformed document: Line points");
                return false;
            }
            QJsonArray jsonPoints = jsonLine.toArray();
            QPointF points[2];
            for (int i = 0; i < 2; i++) {
                QJsonValue jsonPoint = jsonPoints.at(i);
                if (!jsonPoint.isArray() || jsonPoint.toArray().count() != 2 ||
                       !jsonPoint.toArray().at(0).isDouble() ||
                       !jsonPoint.toArray().at(1).isDouble()) {
                    *errorString = QString("Malformed document: Line point %1").arg(i);
                    return false;
                }
                points[i].setX(jsonPoint.toArray().at(0).toDouble());
                points[i].setY(jsonPoint.toArray().at(0).toDouble());
            }
            rectItem->setRect(points[0], points[1]);
            item = rectItem;
        }
        else if (type == "polyline") {
            GraphicsLine *lineItem = new GraphicsLine();
            QJsonValue jsonLine = jsonObject.value("points");
            if (!jsonLine.isArray() || jsonLine.toArray().count() != 2) {
                *errorString = QString("Malformed document: Line points");
                return false;
            }
            QJsonArray jsonPoints = jsonLine.toArray();
            QPointF points[2];
            for (int i = 0; i < 2; i++) {
                QJsonValue jsonPoint = jsonPoints.at(i);
                if (!jsonPoint.isArray() || jsonPoint.toArray().count() != 2 ||
                       !jsonPoint.toArray().at(0).isDouble() ||
                       !jsonPoint.toArray().at(1).isDouble()) {
                    *errorString = QString("Malformed document: Line point %1").arg(i);
                    return false;
                }
                points[i].setX(jsonPoint.toArray().at(0).toDouble());
                points[i].setY(jsonPoint.toArray().at(0).toDouble());
            }
            lineItem->setLine(points[0], points[1]);
            item = lineItem;
        }
        item->setPos(pos[0], pos[1]);
        item->setZValue(pos[2]);
    }

    // All done and nice, store data and return true
    m_boardSize = boardSize;
    m_items = items;
    m_layers = layers;

    return true;
}

QSizeF PcbDocument::boardSize() const
{
    return m_boardSize;
}

QList<int> PcbDocument::layers() const
{
    return m_layers;
}

QList<GraphicsItem *> PcbDocument::items() const
{
    return m_items;
}

bool PcbDocument::save(QString *errorString, const QString &fileName)
{
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    return false;
}
