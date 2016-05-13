#ifndef PROPERTYEDITORDOCKWIDGET_H
#define PROPERTYEDITORDOCKWIDGET_H

#include "scheditor/dock/dockwidget.h"

#include <QDockWidget>
#include <QMap>
#include <QList>
#include <QString>

class ObjectPropertyEditor;
class SchItem;
class QStackedWidget;
class QComboBox;

class PropertyEditorDockWidget : public DockWidget
{
    Q_OBJECT

public:
    PropertyEditorDockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

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

    // DockWidget interface
public slots:
    void applySettings(const SchEditorSettings &settings);
};

#endif // PROPERTYEDITORDOCKWIDGET_H
