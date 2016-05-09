#ifndef ABSTRACTEDITOR_H
#define ABSTRACTEDITOR_H

#include "core/core_global.h"

#include <QWidget>

class QMainWindow;
class QSettings;

// FIXME: Get rid of this, activate/desactivate is part of IEditor,
//  and read/write settings should be manage in a custom way by plugins
class CORE_EXPORT AbstractEditor : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractEditor(QWidget *parent = nullptr);
    ~AbstractEditor();

    virtual void activate(QMainWindow *window) = 0;
    virtual void desactivate(QMainWindow *window) = 0;

    virtual void readSettings(QSettings &settings) = 0;
    virtual void writeSettings(QSettings &settings) = 0;

    virtual QString type() const = 0;
signals:

public slots:
};

#endif // ABSTRACTEDITOR_H
