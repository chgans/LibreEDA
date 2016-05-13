#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include "scheditordocument.h"

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
    void addDocumentItem(quint64 id, const SchEditorDocument::Item *item);
    void removeDocumentItem(quint64 id);

private:
    QMap<quint64, SchItem*> m_itemMap;
};

#endif // GRAPHICSSCENE_H
