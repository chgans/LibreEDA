#ifndef PROPERTYEDITORDOCKWIDGET_H
#define PROPERTYEDITORDOCKWIDGET_H

#include <QDockWidget>

class QtTreePropertyBrowser;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class PenPropertyManager;
class PenEditorFactory;
class QtProperty;
class SchItem;

class PropertyEditorDockWidget : public QDockWidget
{
public:
    PropertyEditorDockWidget();

signals:

public slots:
    void setObject(QObject *object);
    void setItem(SchItem *item);

private:
    QtTreePropertyBrowser *m_browser;
    QtVariantPropertyManager *m_manager;
    QtVariantEditorFactory *m_factory;
    PenPropertyManager *m_penManager;
    PenEditorFactory *m_penFactory;
    QObject *m_object;
    SchItem *m_item;
    QMap<QtProperty *, int> m_propertyToMetaPropertyIndex;

    bool m_populatingBrowser;

    void populateBrowser(QObject *object, const QMetaObject *metaObject);
    void populateBrowser(SchItem *item);

    //QMap<int, int> m_enumIndexToIntValue;

private slots:
    void setObjectPropertyValue(QtProperty *property, const QVariant &value);
    void setBrowserPropertyValue(QtProperty *property, const QVariant &value);
};

#endif // PROPERTYEDITORDOCKWIDGET_H
