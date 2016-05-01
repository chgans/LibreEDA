#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include <QGraphicsView>

class Scene;
class GraphicsItem;
class LayoutView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LayoutView(Scene *scene, QWidget *parent = nullptr);

    Scene *layoutScene() const;

    virtual QColor colorForItem(const GraphicsItem *item) const;
    virtual qreal opacityForItem(const GraphicsItem *item) const;
    virtual bool shouldPaintItem(const GraphicsItem *item) const;
signals:

public slots:

};

#endif // LAYOUTVIEW_H
