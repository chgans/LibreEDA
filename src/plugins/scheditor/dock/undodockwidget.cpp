#include "undodockwidget.h"

#include <QUndoView>
#include <QUndoStack>

UndoDockWidget::UndoDockWidget(QWidget *parent, Qt::WindowFlags flags):
    DockWidget(parent, flags),
    m_view(new QUndoView)
{
    setWindowTitle("Undo/Redo Stack");
    setWidget(m_view);
}

void UndoDockWidget::setStack(QUndoStack *stack)
{
    m_view->setStack(stack);
}

void UndoDockWidget::applySettings(const Settings &settings)
{
    Q_UNUSED(settings);
}
