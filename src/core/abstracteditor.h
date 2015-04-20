#ifndef ABSTRACTEDITOR_H
#define ABSTRACTEDITOR_H

#include <QWidget>

class QMainWindow;
class QSettings;

class AbstractEditor : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractEditor(QWidget *parent = 0);
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
