#ifndef PROPERTYEDITORDOCKWIDGET_H
#define PROPERTYEDITORDOCKWIDGET_H

#include <QDockWidget>
#include <QMap>
#include <QList>
#include <QString>

class ObjectPropertyEditor;
class SchItem;
class QStackedWidget;
class QComboBox;

class PropertyEditorDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditorDockWidget();

signals:

public slots:
    void setObjects(QList<QObject *> objects);
    void setItem(SchItem *item);

private slots:
    void updateEditor(int index);

private:
    ObjectPropertyEditor *m_editor;
    QComboBox *m_objectComboBox;
    QMap<QString, QList<QObject *> > m_classNameToObjects;
    QList<QObject *> m_objects;
};

#endif // PROPERTYEDITORDOCKWIDGET_H
