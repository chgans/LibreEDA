#include "layoutview.h"
#include "scene.h"

LayoutView::LayoutView(Scene *scene, QWidget *parent) :
    QGraphicsView(parent)
{
    setScene(scene);
}

Scene *LayoutView::layoutScene() const
{
    return static_cast<Scene *>(scene());
}

QColor LayoutView::colorForItem(const GraphicsItem *item) const
{
    Q_UNUSED(item);
    return QColor(Qt::red);
}

// Unlike color, the scene need to know in advance the item opacity
qreal LayoutView::opacityForItem(const GraphicsItem *item) const
{
    Q_UNUSED(item);
    return 0.75;
}

bool LayoutView::shouldPaintItem(const GraphicsItem *item) const
{
    Q_UNUSED(item);
    return true;
}
