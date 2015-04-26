#ifndef ABSTRACTGRAPHICSINTERACTIVETOOL_H
#define ABSTRACTGRAPHICSINTERACTIVETOOL_H

#include "schtool.h"

class QMouseEvent;
class QKeyEvent;
class QWheelEvent;
class QWidget;

class SchItem;

class AbstractGraphicsInteractiveTool: public SchTool
{
    Q_OBJECT

public:
    AbstractGraphicsInteractiveTool(QObject *parent = 0);
    ~AbstractGraphicsInteractiveTool();

    QWidget *optionWidget();
    void setOptionWidget(QWidget *widget);
    QWidget *taskWidget();
    void setOperationWidget(QWidget *widget);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void wheelEvent(QWheelEvent *event);

protected:
    QWidget *m_taskWidget;
    QWidget *m_optionWidget;

    SchItem *createPhantomItem(SchItem *item);
    QList<SchItem *> createPhantomItems(const QList<SchItem *> &items);
};

#endif // ABSTRACTGRAPHICSINTERACTIVETOOL_H
