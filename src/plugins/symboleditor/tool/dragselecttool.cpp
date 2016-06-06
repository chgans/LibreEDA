#include "dragselecttool.h"
#include "view/view.h"
#include "view/scene.h"

#include <QRectF>
#include <QRubberBand>
#include <QMouseEvent>

namespace SymbolEditor
{

    DragSelectTool::DragSelectTool(QObject *parent):
        InteractiveTool (parent),
        m_rubberBand(new QRubberBand(QRubberBand::Rectangle))
    {

    }

    void DragSelectTool::mousePressEvent(QMouseEvent *event)
    {
        m_pressPosition = event->pos();
        m_rubberBand->setParent(view()->viewport());
        m_rubberBand->setGeometry(QRect());
        m_rubberBand->show();
    }

    void DragSelectTool::mouseMoveEvent(QMouseEvent *event)
    {
        QRect viewRect = view()->mapFromScene(QRectF(m_pressPosition, event->pos())).boundingRect();
        m_rubberBand->setGeometry(viewRect);
        QPainterPath path;
        path.addPolygon(view()->mapToScene(m_rubberBand->geometry()));
        scene()->setSelectionArea(path);
    }

    void DragSelectTool::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event);

        QPainterPath path;
        path.addPolygon(view()->mapToScene(m_rubberBand->geometry()));
        scene()->setSelectionArea(path);
        m_rubberBand->setParent(nullptr);
        m_rubberBand->hide();
    }


    void DragSelectTool::activate(View *view)
    {
        setView(view);
    }

    void DragSelectTool::desactivate()
    {
    }

}
