#ifndef ISETTINGSPAGE_H
#define ISETTINGSPAGE_H

#include <QObject>
#include <QIcon>

class ISettingsPage : public QObject
{
    Q_OBJECT
public:
    explicit ISettingsPage(QObject *parent = nullptr);

    QString pageName() const;
    QString categoryName() const;
    QIcon categoryIcon() const;

    virtual QWidget *widget() = 0;
    virtual void apply() = 0;
    virtual void finish() = 0;

protected:
    void setPageName(const QString &name);
    void setCategoryName(const QString &name);
    void setCategoryIcon(const QIcon &icon);

private:
    QString m_pageName;
    QString m_categoryName;
    QIcon m_categoryIcon;
};

#endif // ISETTINGSPAGE_H
