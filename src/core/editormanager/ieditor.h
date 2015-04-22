#ifndef IEDITOR_H
#define IEDITOR_H

#include <QObject>
#include <QPointer>

class IDocument;
class QSettings;
class QWidget;

class IEditor : public QObject
{
    Q_OBJECT
public:
    explicit IEditor(QObject *parent = 0);
    ~IEditor();

    virtual bool open(QString *errorString, const QString &fileName) = 0;
    virtual IDocument *document() = 0;

    virtual void saveState(QSettings *settings) const;
    virtual bool restoreState(QSettings *settings);

    virtual QWidget *widget() const;
    virtual void setWidget(QWidget *widget);

private:
    QPointer<QWidget> m_widget;
};

#endif // IEDITOR_H
