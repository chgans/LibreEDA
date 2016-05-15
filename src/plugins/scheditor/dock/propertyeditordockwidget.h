#ifndef PROPERTYEDITORDOCKWIDGET_H
#define PROPERTYEDITORDOCKWIDGET_H

#include "scheditor/dock/dockwidget.h"

#include <QDockWidget>

class ItemPropertyEditor;
class SchItem;

class PropertyEditorDockWidget : public DockWidget
{
    Q_OBJECT

public:
    PropertyEditorDockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

signals:

public slots:
    void setItem(SchItem *item);

private:
    ItemPropertyEditor *m_editor;

    // DockWidget interface
public slots:
    void applySettings(const SchEditorSettings &settings);
};

#endif // PROPERTYEDITORDOCKWIDGET_H
