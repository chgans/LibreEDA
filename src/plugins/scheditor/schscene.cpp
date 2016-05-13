#include "schscene.h"

#include "item/graphicscircleitem.h"
#include "item/graphicsrectitem.h"
#include "item/graphicsellipseitem.h"
#include "item/graphicspolygonitem.h"

#include <QPainter>

SchScene::SchScene(QObject *parent):
    QGraphicsScene(parent)
{
}

SchScene::~SchScene()
{
}

QList<SchItem *> SchScene::selectedObjects()
{
    QList<SchItem *> objects;
    for (QGraphicsItem *item: selectedItems()) {
        SchItem *object = dynamic_cast<SchItem *>(item);
        if (object != nullptr)
            objects.append(object);
    }
    return objects;
}

void SchScene::applySettings(const SchEditorSettings &settings)
{
    Q_UNUSED(settings);
}

void SchScene::addDocumentItem(quint64 id, const SchEditorDocument::Item *item)
{
    switch (item->type())
    {
        case xdl::symbol::Item::Rectangle:
        {
            auto documentItem = reinterpret_cast<const xdl::symbol::RectangleItem*>(item);
            auto sceneItem = new GraphicsRectItem();
            sceneItem->setRect(QRectF(documentItem->topLeft, documentItem->bottomRight));
            sceneItem->setPos(documentItem->position);
            sceneItem->setRoundness(1.0, 1.0);
            sceneItem->setPen(documentItem->pen);
            sceneItem->setBrush(documentItem->brush);
            sceneItem->setData(0, QVariant::fromValue<quint64>(id));
            m_itemMap.insert(id, sceneItem);
            addItem(sceneItem);
            break;
        }
        case xdl::symbol::Item::Circle:
        {
            auto documentItem = reinterpret_cast<const xdl::symbol::CircleItem*>(item);
            auto sceneItem = new GraphicsCircleItem;
            sceneItem->setPos(documentItem->position);
            sceneItem->setRadius(documentItem->radius);
            sceneItem->setPen(documentItem->pen);
            sceneItem->setBrush(documentItem->brush);
            sceneItem->setData(0, QVariant::fromValue<quint64>(id));
            m_itemMap.insert(id, sceneItem);
            addItem(sceneItem);
            break;
        }
        case xdl::symbol::Item::CircularArc:
        {
            return;
        }
        case xdl::symbol::Item::Ellipse:
        {
            auto documentItem = reinterpret_cast<const xdl::symbol::EllipseItem*>(item);
            auto sceneItem = new GraphicsEllipseItem;
            sceneItem->setPos(documentItem->position);
            sceneItem->setXRadius(documentItem->xRadius);
            sceneItem->setYRadius(documentItem->yRadius);
            sceneItem->setPen(documentItem->pen);
            sceneItem->setBrush(documentItem->brush);
            sceneItem->setData(0, QVariant::fromValue<quint64>(id));
            m_itemMap.insert(id, sceneItem);
            addItem(sceneItem);
            break;
        }
        case xdl::symbol::Item::EllipticalArc:
        {
            return;
        }
        case xdl::symbol::Item::Polyline:
        {
            return;
        }
        case xdl::symbol::Item::Polygon:
        {
            auto documentItem = reinterpret_cast<const xdl::symbol::PolygonItem*>(item);
            auto sceneItem = new GraphicsPolygonItem;
            sceneItem->setPos(documentItem->position);
            sceneItem->setPen(documentItem->pen);
            sceneItem->setBrush(documentItem->brush);
            sceneItem->setPolygon(QPolygonF(documentItem->vertices.toVector())); // FIXME: use QVector in xdl too?
            sceneItem->setData(0, QVariant::fromValue<quint64>(id));
            m_itemMap.insert(id, sceneItem);
            addItem(sceneItem);
            break;
        }
        case xdl::symbol::Item::Label:
        {
            return;
        }
        case xdl::symbol::Item::Pin:
        {
            return;
        }
        case xdl::symbol::Item::Group:
        {
            return;
        }
    }
}

void SchScene::updateDocumentItem(quint64 id, const SchEditorDocument::Item *item)
{
    if (!m_itemMap.contains(id))
    {
        return;
    }

    auto sceneItem = m_itemMap.value(id);
    sceneItem->setPos(item->position);

    switch (item->type())
    {
        case xdl::symbol::Item::Rectangle:
        {
            break;
        }
        case xdl::symbol::Item::Circle:
        {
            break;
        }
        case xdl::symbol::Item::CircularArc:
        {
            return;
        }
        case xdl::symbol::Item::Ellipse:
        {
            break;
        }
        case xdl::symbol::Item::EllipticalArc:
        {
            return;
        }
        case xdl::symbol::Item::Polyline:
        {
            return;
        }
        case xdl::symbol::Item::Polygon:
        {
            break;
        }
        case xdl::symbol::Item::Label:
        {
            return;
        }
        case xdl::symbol::Item::Pin:
        {
            return;
        }
        case xdl::symbol::Item::Group:
        {
            return;
        }
    }
}

void SchScene::removeDocumentItem(quint64 id)
{
    if (!m_itemMap.contains(id))
    {
        return;
    }

    auto sceneItem = m_itemMap.value(id);
    removeItem(sceneItem);
    delete sceneItem;
}

