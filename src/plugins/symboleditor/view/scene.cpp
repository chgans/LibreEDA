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
            sceneItem->setRect(QRectF(0, 0, documentItem->width(), documentItem->height()));
            sceneItem->setPos(documentItem->position());
            sceneItem->setRoundness(1.0, 1.0);
            sceneItem->setPen(makePen(documentItem));
            sceneItem->setBrush(makeBrush(documentItem));
            sceneItem->setData(0, QVariant::fromValue<quint64>(id));
            m_itemMap.insert(id, sceneItem);
            addItem(sceneItem);
            break;
        }
        case xdl::symbol::Item::Circle:
        {
            auto documentItem = reinterpret_cast<const xdl::symbol::CircleItem *>(item);
            auto sceneItem = new CircleItem;
            sceneItem->setPos(documentItem->position());
            sceneItem->setRadius(documentItem->radius());
            sceneItem->setPen(makePen(documentItem));
            sceneItem->setBrush(makeBrush(documentItem));
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
            sceneItem->setPos(documentItem->position());
            sceneItem->setXRadius(documentItem->xRadius());
            sceneItem->setYRadius(documentItem->yRadius());
            sceneItem->setPen(makePen(documentItem));
            sceneItem->setBrush(makeBrush(documentItem));
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
            sceneItem->setPos(documentItem->position());
            sceneItem->setPen(makePen(documentItem));
            sceneItem->setBrush(makeBrush(documentItem));
            sceneItem->setPolygon(QPolygonF(
                                      documentItem->vertices().toVector())); // FIXME: use QVector in xdl too?
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
    sceneItem->setPos(item->position());

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

void Scene::updateDocumentItemProperty(quint64 itemId, quint64 propertyId, const QVariant &value)
{
    // FIXME: Scene's Item need to understand property ID by defining compatible enums
    // This way Scene's Item are independent of xdl:symbol
    auto item = itemForDocumentId(itemId);
    if (item == nullptr)
    {
        return;
    }

    // TODO: item->setProperty(propertyId, value);
    switch (propertyId)
    {
        case xdl::symbol::Item::PositionProperty:
            item->setPos(value.toPointF());
            break;
        case xdl::symbol::Item::VisibilityProperty:
            item->setVisible(value.toBool());
            break;
        case xdl::symbol::Item::LockedProperty:
            item->setEnabled(!value.toBool());
            break;
        default:
            break;
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

QPen Scene::makePen(const Document::Item *item)
{
    QPen pen;
    pen.setStyle(Qt::PenStyle(item->lineStyle()));
    qreal width;
    switch (item->lineWidth())
    {
        case xdl::symbol::ThinestLine:
            width = 0.13;
            break;
        case xdl::symbol::ThinerLine:
            width = 0.18;
            break;
        case xdl::symbol::ThinLine:
            width = 0.25;
            break;
        case xdl::symbol::SlightlyThinLine:
            width = 0.35;
            break;
        case xdl::symbol::MediumLine:
            width = 0.50;
            break;
        case xdl::symbol::SlightlyThickLine:
            width = 0.70;
            break;
        case xdl::symbol::ThickLine:
            width = 1.0;
            break;
        case xdl::symbol::ThickerLine:
            width = 1.40;
            break;
        case xdl::symbol::ThickestLine:
            width = 2.00;
            break;
    }
    pen.setWidthF(width);
    return pen;
}

QBrush Scene::makeBrush(const Document::Item *item)
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(item->fillColor());
    return brush;
}

