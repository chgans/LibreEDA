#ifndef ABSTRACTEDITOR_H
#define ABSTRACTEDITOR_H

#include <QWidget>

class QMainWindow;

class AbstractEditor : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractEditor(QWidget *parent = 0);
    ~AbstractEditor();

    virtual void activate(QMainWindow *window) = 0;
    virtual void desactivate(QMainWindow *window) = 0;

signals:

public slots:
};

#endif // ABSTRACTEDITOR_H
