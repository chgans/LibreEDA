#ifndef IEDITOR_H
#define IEDITOR_H

#include "core/core_global.h"

#include <QObject>
#include <QPointer>
#include <QIcon>

class IDocument;
class QSettings;
class QWidget;
class QMainWindow;

class CORE_EXPORT IEditor : public QObject
{
    Q_OBJECT
public:
    explicit IEditor(QObject *parent = nullptr);
    ~IEditor();

    virtual bool open(QString *errorString, const QString &fileName) = 0;
    virtual IDocument *document() const = 0;

    virtual void saveState(QSettings *settings) const;
    virtual bool restoreState(QSettings *settings);

    virtual QWidget *widget() const;
    virtual void setWidget(QWidget *widget);

    virtual QIcon icon() const;
    virtual void setIcon(const QIcon &icon);
    virtual QString displayName() const;
    virtual void setDisplayName(const QString &displayName);

    virtual void activate(QMainWindow *mainWindow) = 0;
    virtual void desactivate(QMainWindow *mainWindow) = 0;

private:
    QPointer<QWidget> m_widget;
    QIcon m_icon;
    QString m_displayName;
};

#endif // IEDITOR_H
