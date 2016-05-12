#include "tool/schtool.h"

#include "schscene.h"
#include "schview.h"

#include <QDialog>

SchTool::SchTool(QObject *parent) :
    QObject(parent),
    m_view(nullptr)
{

}

SchTool::~SchTool()
{

}

SchView *SchTool::view()
{
    return m_view;
}

SchScene *SchTool::scene()
{
    return m_view->scene();
}

void SchTool::setView(SchView *view)
{
    m_view = view;
}

void SchTool::setToolGroup(const QString &group)
{
    m_toolGroup = group;
}

QString SchTool::toolGroup() const
{
    return m_toolGroup;
}

void SchTool::setAction(QAction *action)
{
    m_action = action;
}

QAction *SchTool::action() const
{
    return m_action;
}

void SchTool::cancel()
{

}

void SchTool::applySettings(const SchEditorSettings &settings)
{
    Q_UNUSED(settings);
}
