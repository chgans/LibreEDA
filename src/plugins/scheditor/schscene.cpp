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

