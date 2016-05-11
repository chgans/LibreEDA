#include "symbol.h"

xdl::symbol::Item::Item()
{

}

xdl::symbol::Item::~Item()
{

}

xdl::symbol::Symbol::Symbol()
{

}

xdl::symbol::Symbol::~Symbol()
{
    qDeleteAll(drawingItems);
}

xdl::symbol::RectangleItem::RectangleItem()
{

}

xdl::symbol::RectangleItem::~RectangleItem()
{

}

xdl::symbol::CircleItem::CircleItem()
{

}

xdl::symbol::CircleItem::~CircleItem()
{

}

xdl::symbol::CircularArcItem::CircularArcItem()
{

}

xdl::symbol::CircularArcItem::~CircularArcItem()
{

}

xdl::symbol::EllipseItem::EllipseItem()
{

}

xdl::symbol::EllipseItem::~EllipseItem()
{

}

xdl::symbol::EllipticalArcItem::EllipticalArcItem()
{

}

xdl::symbol::EllipticalArcItem::~EllipticalArcItem()
{

}

xdl::symbol::PolylineItem::PolylineItem()
{

}

xdl::symbol::PolylineItem::~PolylineItem()
{

}

xdl::symbol::PolygonItem::PolygonItem()
{

}

xdl::symbol::PolygonItem::~PolygonItem()
{

}

xdl::symbol::LabelItem::LabelItem()
{

}

xdl::symbol::LabelItem::~LabelItem()
{

}

xdl::symbol::PinItem::PinItem()
{

}

xdl::symbol::PinItem::~PinItem()
{
    delete(designator);
    delete(label);
}

xdl::symbol::ItemGroup::ItemGroup()
{

}

xdl::symbol::ItemGroup::~ItemGroup()
{
    qDeleteAll(children);
}
