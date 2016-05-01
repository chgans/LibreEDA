#ifndef SCHITEMPROPERTYEDITOR_H
#define SCHITEMPROPERTYEDITOR_H

#include <QWidget>

// See https://github.com/qtproject/qttools/blob/dev/src/designer/src/
// - lib/sdk/abstractpropertyeditor.h
// - lib/shared/qdesigner_propertyeditor_p.h
// - components/propertyeditor/propertyeditor.h

class SchItem;

class SchItemPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit SchItemPropertyEditor(QWidget *parent = nullptr);
    virtual ~SchItemPropertyEditor();

    SchItem *object() const;

signals:
    void propertyChanged(const QString &name, const QVariant &value);

public slots:
    void setObject(SchItem *object);
    void setPropertyValue(const QString &name, const QVariant &value, bool changed = true);

private:
//    QtTreePropertyBrowser *m_propertyBrowser;
//    SchItemPropertyManager *m_propertyManager;
//    SchItemEditorFactory *m_editorFactory;
    SchItem *m_object;
};

#endif // SCHITEMPROPERTYEDITOR_H
