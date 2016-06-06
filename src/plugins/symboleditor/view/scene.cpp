#include "scene.h"

#include "item/circleitem.h"
#include "item/rectangleitem.h"
#include "item/ellipseitem.h"
#include "item/polygonitem.h"

#include <QPainter>

using namespace SymbolEditor;

Scene::Scene(QObject *parent):
    QGraphicsScene(parent)
{
}

Scene::~Scene()
{
}

QList<Item *> Scene::selectedObjects()
{
    QList<Item *> objects;
    for (QGraphicsItem *item : selectedItems())
    {
        Item *object = dynamic_cast<Item *>(item);
        if (object != nullptr)
        {
            objects.append(object);
        }
    }
    return objects;
}

Item *Scene::itemForDocumentId(quint64 id) const
{
    return m_itemMap.value(id);
}

void Scene::applySettings(const Settings &settings)
{
    Q_UNUSED(settings);
}

void Scene::addDocumentItem(quint64 id, const Document::Item *item)
{
    switch (item->type())
    {
        case xdl::symbol::Item::Rectangle:
        {
            auto documentItem = reinterpret_cast<const xdl::symbol::RectangleItem *>(item);
            auto sceneItem = new RectangleItem();
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
            auto documentItem = reinterpret_cast<const xdl::symbol::CircleItem *>(item);
            auto sceneItem = new CircleItem;
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
            auto documentItem = reinterpret_cast<const xdl::symbol::EllipseItem *>(item);
            auto sceneItem = new EllipseItem;
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
            auto documentItem = reinterpret_cast<const xdl::symbol::PolygonItem *>(item);
            auto sceneItem = new PolygonItem;
            sceneItem->setPos(documentItem->position);
            sceneItem->setPen(documentItem->pen);
            sceneItem->setBrush(documentItem->brush);
            sceneItem->setPolygon(QPolygonF(
                                      documentItem->vertices.toVector())); // FIXME: use QVector in xdl too?
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

void Scene::updateDocumentItem(quint64 id, const Document::Item *item)
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

void Scene::removeDocumentItem(quint64 id)
{
    if (!m_itemMap.contains(id))
    {
        return;
    }

    auto sceneItem = m_itemMap.value(id);
    removeItem(sceneItem);
    delete sceneItem;
}

