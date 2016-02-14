#ifndef PROPERTYEDITORDOCKWIDGET_H
#define PROPERTYEDITORDOCKWIDGET_H

#include <QDockWidget>

class QtTreePropertyBrowser;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtProperty;

class PropertyEditorDockWidget : public QDockWidget
{
public:
    PropertyEditorDockWidget();

signals:

public slots:
    void setObject(QObject *object);

private:
    QtTreePropertyBrowser *m_browser;
    QtVariantPropertyManager *m_manager;
    QtVariantEditorFactory *m_factory;
    QObject *m_object;
    bool m_populatingBrowser;

    void populateBrowser(QObject *object, const QMetaObject *metaObject);

private slots:
    void assignProperty(QtProperty *property, const QVariant &value);

};

#endif // PROPERTYEDITORDOCKWIDGET_H
