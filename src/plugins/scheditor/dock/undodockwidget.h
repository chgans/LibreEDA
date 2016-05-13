#ifndef UNDODOCKWIDGET_H
#define UNDODOCKWIDGET_H

#include "scheditor/dock/dockwidget.h"

class QUndoView;
class QUndoStack;

class UndoDockWidget : public DockWidget
{
public:
    UndoDockWidget(QWidget *parent = 0, Qt::WindowFlags flags = 0);

public slots:
    void setStack(QUndoStack *stack);

private:
    QUndoView *m_view;

    // DockWidget interface
public slots:
    void applySettings(const SchEditorSettings &settings);
};

#endif // UNDODOCKWIDGET_H
