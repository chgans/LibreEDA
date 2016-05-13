#include "pcbdocument.h"
#include "designlayer.h"
#include "items/graphicsline.h"
#include "items/graphicsrect.h"
#include "core/json.h"

#include <QFile>
#include <QJsonParseError>

PcbDocument::PcbDocument(QObject *parent):
    IDocument(parent)
{
    setModified(true);
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
    if (docType != "pcb") {
        *errorString = QString("Unknown document type: \"%1\"").arg(docType);
        return false;
    }

    QJsonValue jsonValue;

    // Parse board size
    jsonValue = rootObject.value("size");
    if (jsonValue.isUndefined()) {
        *errorString = QString("Malformed document: missing board size");
        return false;
    }
    QSizeF boardSize;
    if (!Json::toSize(errorString, jsonValue, boardSize))
        return false;

    // Parse layers
    jsonValue = rootObject.value("layers");
    if (jsonValue.isUndefined()) {
        *errorString = QString("Malformed document: missing layer list");
        return false;
    }
    QList<int> layers;
    if (!Json::toIntList(errorString, jsonValue, layers))
        return false;

    // Parse items
    jsonValue = rootObject.value("items");
    if (!jsonValue.isArray()) {
        *errorString = QString("Malformed document: missing item list");
        return false;
    }
    QList<GraphicsItem *> items;
    for (QJsonValue value: jsonValue.toArray()) {
        if (!value.isObject()) {
            *errorString = QString("Malformed document: bad item");
            return false;
        }

        QJsonObject jsonObject = value.toObject();
        QString type = jsonObject.value("type").toString();
        GraphicsItem *item;
        if (type == "rectangle") {
            item = new GraphicsRect();
        }
        else if (type == "polyline") {
            item = new GraphicsLine();
        }
        else if (type.isNull()){
            *errorString = QString("Malformed document: missing item type");
            qDeleteAll(items);
            return false;
        }
        else {
            *errorString = QString("Malformed document: bad item type");
            qDeleteAll(items);
            return false;
        }
        if (!item->fromJson(errorString, jsonObject)) {
            qDeleteAll(items);
            return false;
        }
        items.append(item);
    }

    // All done and nice, store data and return true
    m_boardSize = boardSize;
    m_items = items;
    m_layerStack = layers;

    return true;
}

QSizeF PcbDocument::boardSize() const
{
    return m_boardSize;
}

QList<int> PcbDocument::layerStack() const
{
    return m_layerStack;
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

void PcbDocument::render(QPainter *painter)
{
    Q_UNUSED(painter);
}
