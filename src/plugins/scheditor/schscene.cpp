#include "schscene.h"
#include "item/schitem.h"

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

// TBD: remove this, SchView take care of drawing sceneRect() background
void SchScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);
    // FIXME: Get access to the view's palette
    //painter->setPen(QPen(gpalette::Content1, 0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(backgroundBrush());
    painter->drawRect(sceneRect());
}

