import qbs 1.0

LedaLibrary {
    name: "SchEditor"
    
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "opengl"] }
    Depends { name: "Core" }
    Depends { name: "Utils" }

    files: [
        "abstractgraphicshandle.cpp",
        "abstractgraphicshandle.h",
        "abstractgraphicsinserttool.cpp",
        "abstractgraphicsinserttool.h",
        "abstractgraphicsinteractivetool.cpp",
        "abstractgraphicsinteractivetool.h",
        "graphicsbezierhandle.cpp",
        "graphicsbezierhandle.h",
        "graphicsgrid.cpp",
        "graphicsgrid.h",
        "graphicsobject_p.h",
        "graphicsregularhandle.cpp",
        "graphicsregularhandle.h",
        "igraphicsitemobserver.cpp",
        "igraphicsitemobserver.h",
        "igraphicsobservableitem.cpp",
        "igraphicsobservableitem.h",
        "schdocument.cpp",
        "schdocument.h",
        "scheditorconstants.h",
        "scheditor.cpp",
        "scheditorfactory.cpp",
        "scheditorfactory.h",
        "scheditor.h",
        "scheditorwidget.cpp",
        "scheditorwidget.h",
        "schitem.cpp",
        "schitem.h",
        "schscene.cpp",
        "schscene.h",
        "schtool.cpp",
        "schtool.h",
        "schview.cpp",
        "schview.h",
        "dock/taskdockwidget.cpp",
        "dock/taskdockwidget.h",
        "grid/graphicscartesiangrid.cpp",
        "grid/graphicscartesiangrid.h",
        "item/graphicsbezieritem.cpp",
        "item/graphicsbezieritem.h",
        "item/graphicscircleitem.cpp",
        "item/graphicscircleitem.h",
        "item/graphicsellipseitem.cpp",
        "item/graphicsellipseitem.h",
        "item/graphicslineitem.cpp",
        "item/graphicslineitem.h",
        "item/graphicspolygonitem.cpp",
        "item/graphicspolygonitem.h",
        "item/graphicsrectitem.cpp",
        "item/graphicsrectitem.h",
        "item/graphicswireitem.cpp",
        "item/graphicswireitem.h",
        "tool/graphicsbeziertool.cpp",
        "tool/graphicsbeziertool.h",
        "tool/graphicscircletool.cpp",
        "tool/graphicscircletool.h",
        "tool/graphicsellipsetool.cpp",
        "tool/graphicsellipsetool.h",
        "tool/graphicsitemaligntool.cpp",
        "tool/graphicsitemaligntool.h",
        "tool/graphicslinetool.cpp",
        "tool/graphicslinetool.h",
        "tool/graphicspolygontool.cpp",
        "tool/graphicspolygontool.h",
        "tool/graphicsrecttool.cpp",
        "tool/graphicsrecttool.h",
        "tool/graphicsselecttool.cpp",
        "tool/graphicsselecttool.h",
        "tool/graphicswiretool.cpp",
        "tool/graphicswiretool.h",
        "widget/coordinatewidget.cpp",
        "widget/coordinatewidget.h",
        "widget/taskeditwidget.cpp",
        "widget/taskeditwidget.h",
        "widget/taskoptionwidget.cpp",
        "widget/taskoptionwidget.h",
        "widget/wireoperationwidget.cpp",
        "widget/wireoperationwidget.h",

    ]
}
