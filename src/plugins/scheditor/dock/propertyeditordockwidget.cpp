#include "propertyeditordockwidget.h"
#include "propertyeditor/objectpropertyeditor.h"

#include <QComboBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QMap>

PropertyEditorDockWidget::PropertyEditorDockWidget():
    QDockWidget("Property editor")
{
    QWidget *widget = new QWidget;
    setWidget(widget);
    widget->setLayout(new QVBoxLayout);

    m_objectComboBox = new QComboBox;
    widget->layout()->addWidget(m_objectComboBox);
    m_editor = new ObjectPropertyEditor;
    widget->layout()->addWidget(m_editor);

    connect(m_objectComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &PropertyEditorDockWidget::updateEditor);
}

void PropertyEditorDockWidget::setObjects(QList<QObject *> objects)
{
    m_objects = objects;
    m_objectComboBox->clear();

    m_classNameToObjects.clear();
    foreach (QObject *object, objects) {
        QString className(object->metaObject()->className());
        m_classNameToObjects[className].append(object);
    }
    m_objectComboBox->addItem(QString("All (%2)").arg(m_objects.count()),
                              QVariant::fromValue<QList<QObject *> >(m_objects));
    foreach (const QString &className, m_classNameToObjects.keys()) {
        QList<QObject *> objects = m_classNameToObjects[className];
        m_objectComboBox->addItem(QString("%1 (%2)").arg(className).arg(objects.count()),
                                  QVariant::fromValue<QList<QObject *> >(objects));
    }
    if (!objects.isEmpty()) {
        m_objectComboBox->setCurrentIndex(0);
        updateEditor(0);
    }
}

void PropertyEditorDockWidget::setItem(SchItem */*item*/)
{

}

void PropertyEditorDockWidget::updateEditor(int)
{
    if (m_objectComboBox->count() == 0) {
        m_editor->setObjects(QList<QObject *>());
    }
    else {
        QList<QObject *> objects = m_objectComboBox->currentData().value<QList<QObject *> >();
        m_editor->setObjects(objects);
    }
}
