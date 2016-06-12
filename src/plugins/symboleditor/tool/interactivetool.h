#pragma once

#include "tool/tool.h"

#include <QList>

class QMouseEvent;
class QKeyEvent;
class QWheelEvent;
class QWidget;

namespace SymbolEditor
{

    class InteractiveTool: public Tool
    {
        Q_OBJECT

    public:
        explicit InteractiveTool(Editor *editor = nullptr);
        ~InteractiveTool();

        QList<QWidget *> taskWidgets();
        void setTaskWidgets(QList<QWidget *> widgets);

        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void mouseDoubleClickEvent(QMouseEvent *event);

        virtual void keyPressEvent(QKeyEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);

        virtual void wheelEvent(QWheelEvent *event);

    signals:
        void taskWidgetsChanged(QList<QWidget *> widgets);

    protected:
        QList<QWidget *> m_taskWidgets;
    };

}
