#ifndef OBJECTPREVIEW_H
#define OBJECTPREVIEW_H

#include <QGraphicsView>

// Shows graphical preview of the object under mouse cursor
// Typically from the library
class ObjectPreview : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ObjectPreview(QWidget *parent = nullptr);

    virtual void setObjectToPreview(QGraphicsItem *item);

signals:

public slots:

protected:
    void drawItems(QPainter *painter, int numItems,
                    QGraphicsItem *items[],
                    const QStyleOptionGraphicsItem options[]);
    void resizeEvent(QResizeEvent *event);

private:
    QGraphicsItem *m_object;
};

#endif // OBJECTPREVIEW_H
