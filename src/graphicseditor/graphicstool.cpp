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

void GraphicsTool::setToolGroup(const QString &group)
{
    m_toolGroup = group;
}

QString GraphicsTool::toolGroup() const
{
    return m_toolGroup;
}

void GraphicsTool::setAction(QAction *action)
{
    m_action = action;
}

QAction *GraphicsTool::action() const
{
    return m_action;
}

void GraphicsTool::cancel()
{

}


