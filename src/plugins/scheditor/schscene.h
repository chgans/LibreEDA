#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class SchItem;
class SchEditorSettings;

class SchScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit SchScene(QObject *parent = nullptr);
    ~SchScene();

    QList<SchItem *> selectedObjects();

public slots:
    void applySettings(const SchEditorSettings &settings);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

};

#endif // GRAPHICSSCENE_H
