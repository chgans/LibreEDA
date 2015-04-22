#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>

class ActionManager : public QObject
{
    Q_OBJECT
public:
    explicit ActionManager(QObject *parent = 0);
    ~ActionManager();

signals:

public slots:
};

#endif // ACTIONMANAGER_H
