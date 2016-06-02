#pragma once

#include <QTreeView>

namespace SymbolEditor
{

    class ObjectInspectorView: public QTreeView
    {
        Q_OBJECT

    public:
        ObjectInspectorView(QWidget *parent = nullptr);
    };

}
