#include "tool/interactivetool.h"

#include "schscene.h"
#include "schview.h"
#include "item/schitem.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QGraphicsColorizeEffect>

InteractiveTool::InteractiveTool(QObject *parent):
    SchTool(parent)
{

}

InteractiveTool::~InteractiveTool()
{

}

QWidget *InteractiveTool::optionWidget()
{
    return m_optionWidget;
}

void InteractiveTool::setOptionWidget(QWidget *widget)
{
    m_optionWidget = widget;
}

QWidget *InteractiveTool::taskWidget()
{
    return m_taskWidget;
}

void InteractiveTool::setOperationWidget(QWidget *widget)
{
    m_taskWidget = widget;
}

QList<QWidget *> InteractiveTool::optionWidgets()
{
    return m_optionWidgets;
}

void InteractiveTool::setOptionWidgets(QList<QWidget *> widgets)
{
    m_optionWidgets = widgets;
}

void InteractiveTool::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void InteractiveTool::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}

SchItem *InteractiveTool::createPhantomItem(SchItem *item)
{
    SchItem *phantomItem = item->clone();
    // Pity we cannot mix effects, drop shadow is nice while moving/clonig
    // But not good while moving handles with the select tool
    QGraphicsColorizeEffect *phantomEffect = new QGraphicsColorizeEffect();
    phantomEffect->setColor(Qt::darkGray);
    phantomEffect->setStrength(.5);
    phantomEffect->setEnabled(true);
    //phantomItem->setOpacity(phantomItem->opacity()*0.9);
    phantomItem->setGraphicsEffect(phantomEffect);
    scene()->addItem(phantomItem);
    phantomItem->setSelected(false); // force deselected
    return phantomItem;
}

QList<SchItem *> InteractiveTool::createPhantomItems(const QList<SchItem *> &items)
{
    QList<SchItem *> phantomItems;
    for (SchItem *item : items)
    {
        phantomItems.append(createPhantomItem(item));
    }
    return phantomItems;
}
