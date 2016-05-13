#include "schscene.h"
#include "item/schitem.h"

#include "item/graphicscircleitem.h"
#include "item/graphicsrectitem.h"
#include "item/graphicsellipseitem.h"

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
    foreach (QGraphicsItem *item, selectedItems()) {
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
            m_itemMap.insert(id, sceneItem);
            addItem(sceneItem);
            break;
        }
        case xdl::symbol::Item::Circle:
        {
            break;
        }
        case xdl::symbol::Item::CircularArc:
            break;
        case xdl::symbol::Item::Ellipse:
        {
            break;
        }
        case xdl::symbol::Item::EllipticalArc:
            break;
        case xdl::symbol::Item::Polyline:
            break;
        case xdl::symbol::Item::Polygon:
            break;
        case xdl::symbol::Item::Label:
            break;
        case xdl::symbol::Item::Pin:
            break;
        case xdl::symbol::Item::Group:
            break;
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

