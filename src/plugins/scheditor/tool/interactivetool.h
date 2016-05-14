#ifndef INTERACTIVETOOL_H
#define INTERACTIVETOOL_H

#include "tool/schtool.h"

#include <QList>

class QMouseEvent;
class QKeyEvent;
class QWheelEvent;
class QWidget;

class SchItem;

class InteractiveTool: public SchTool
{
    Q_OBJECT

public:
    explicit InteractiveTool(QObject *parent = nullptr);
    ~InteractiveTool();

    QList<QWidget *> optionWidgets();
    void setOptionWidgets(QList<QWidget *> widgets);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void wheelEvent(QWheelEvent *event);

protected:
    QList<QWidget *> m_optionWidgets;

    SchItem *createPhantomItem(SchItem *item);
    QList<SchItem *> createPhantomItems(const QList<SchItem *> &items);
};

#endif // INTERACTIVETOOL_H
