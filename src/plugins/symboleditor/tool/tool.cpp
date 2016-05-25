#include "tool/tool.h"

#include "view/scene.h"
#include "view/view.h"

#include <QDialog>

using namespace SymbolEditor;

Tool::Tool(QObject *parent) :
    QObject(parent),
    m_view(nullptr)
{

}

Tool::~Tool()
{

}

View *Tool::view()
{
    return m_view;
}

Scene *Tool::scene()
{
    return m_view->scene();
}

void Tool::setView(View *view)
{
    m_view = view;
}

void Tool::setToolGroup(const QString &group)
{
    m_toolGroup = group;
}

QString Tool::toolGroup() const
{
    return m_toolGroup;
}

void Tool::setAction(QAction *action)
{
    m_action = action;
}

QAction *Tool::action() const
{
    return m_action;
}

void Tool::cancel()
{

}

void Tool::applySettings(const Settings &settings)
{
    Q_UNUSED(settings);
}
