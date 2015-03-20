#include "graphicstool.h"

#include "graphicsscene.h"
#include "graphicsview.h"

#include <QDialog>

GraphicsTool::GraphicsTool(QObject *parent) :
    QObject(parent),
    m_view(nullptr)
{

}

GraphicsTool::~GraphicsTool()
{

}

GraphicsView *GraphicsTool::view()
{
    return m_view;
}

GraphicsScene *GraphicsTool::scene()
{
    return m_view->scene();
}

void GraphicsTool::setView(GraphicsView *view)
{
    m_view = view;
}

//void GraphicsTool::keyPressEvent(QKeyEvent *event)
//{
//    switch (event->key()) {
//    case Qt::Key_Escape:
//        cancel();
//        break;
//    case Qt::Key_Tab:
//        QDialog *dialog = optionDialog();
//        if (dialog)
//            dialog->exec();
//        break;
//    }
//}

void GraphicsTool::cancel()
{

}

// TODO: the original should be dimmed out instead

