#ifndef INAVIGATIONVIEWFACTORY_H
#define INAVIGATIONVIEWFACTORY_H

#include "core/core_global.h"

#include <QObject>
#include <QKeySequence>

class QToolButton;

struct CORE_EXPORT NavigationView
{
    NavigationView(QWidget *w = 0) : widget(w) {}

    QWidget *widget;
    QList<QWidget *> toolBarWidgets;
};

Q_DECLARE_METATYPE(NavigationView*)

class CORE_EXPORT INavigationViewFactory : public QObject
{
    Q_OBJECT
public:
    explicit INavigationViewFactory(QObject *parent = 0);
    ~INavigationViewFactory();

    void setDisplayName(const QString &displayName);
    void setPriority(int priority);
    void setId(QString id);
    void setActivationSequence(const QKeySequence &keys);

    QString displayName() const;
    int priority() const;
    QString id() const;
    QKeySequence activationSequence() const;

    virtual NavigationView *createView() = 0;

    virtual void saveSettings(int position, QWidget *widget);
    virtual void restoreSettings(int position, QWidget *widget);

signals:

public slots:

private:
    QString m_displayName;
    int m_priority;
    QString m_id;
    QKeySequence m_activationSequence;
};

#endif // INAVIGATIONVIEWFACTORY_H
