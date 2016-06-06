#pragma once

#include "tool/interactivetool.h"

#include <QPointF>

namespace SymbolEditor
{

    class Item;

    class CloneItemTool : public InteractiveTool
    {
    public:
        CloneItemTool(QObject *parent = nullptr);

    protected:
        Item *cloneItem(Item *item);
        QList<Item *> cloneItems(QList<Item *> items);

    private:
        void resetTool();
        enum State
        {
            ChooseOrigin,
            ChooseDestination
        };
        State m_state;
        QList<Item *> m_items;
        QList<Item *> m_clonedItems;
        QPointF m_pressPosition;
        QPointF m_lastPosition;

        // InteractiveTool interface
    public:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);

        // Tool interface
    public:
        void activate(View *view);
        void desactivate();
    };

}
