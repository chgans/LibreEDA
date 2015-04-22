#include "ieditor.h"

#include <QWidget>

IEditor::IEditor(QObject *parent) : QObject(parent)
{

}

IEditor::~IEditor()
{

}

void IEditor::saveState(QSettings *) const
{

}

bool IEditor::restoreState(QSettings *)
{
    return true;
}

QWidget *IEditor::widget() const
{
    return m_widget;
}

void IEditor::setWidget(QWidget *widget)
{
    m_widget = widget;
}

