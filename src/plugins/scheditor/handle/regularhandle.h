#pragma once

#include "handle/handle.h"
#include <QGraphicsPathItem>

namespace SymbolEditor
{

    class RegularHandle : public Handle
    {
    public:
        explicit RegularHandle(Item *parent = nullptr);
        ~RegularHandle();

    };

}
