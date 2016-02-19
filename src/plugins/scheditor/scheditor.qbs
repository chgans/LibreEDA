import qbs 1.0

LedaPlugin {
    name: "SchEditor"
    
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "opengl", "xml"] }
    Depends { name: "Utils" }
    Depends { name: "Core" }
    Depends { name: "QtPropertyBrowser" }

    files: [
        "dock/taskdockwidget.cpp",
        "dock/taskdockwidget.h",
        "dock/propertyeditordockwidget.cpp",
        "dock/propertyeditordockwidget.h",
        "document/circledata.cpp",
        "document/circledata.h",
        "document/circulararcdata.cpp",
        "document/circulararcdata.h",
        "document/documentreader.cpp",
        "document/documentreader.h",
        "document/ellipsedata.cpp",
        "document/ellipsedata.h",
        "document/ellipticarcdata.cpp",
        "document/ellipticarcdata.h",
        "document/graphicsitemdata.cpp",
        "document/graphicsitemdata.h",
        "document/graphicsscenedata.cpp",
        "document/graphicsscenedata.h",
        "document/linedata.cpp",
        "document/linedata.h",
        "document/polygondata.cpp",
        "document/polygondata.h",
        "document/rectangledata.cpp",
        "document/rectangledata.h",
        "document/schdocumentgraphicsobject.cpp",
        "document/schdocumentgraphicsobject.h",
        "document/schdocumentobject.cpp",
        "document/schdocumentobject.h",
        "document/schdocument.cpp",
        "document/schdocument.h",
        "grid/graphicsgrid.cpp",
        "grid/graphicsgrid.h",
        "grid/graphicscartesiangrid.cpp",
        "grid/graphicscartesiangrid.h",
        "handle/abstractgraphicshandle.cpp",
        "handle/abstractgraphicshandle.h",
        "handle/graphicsbezierhandle.cpp",
        "handle/graphicsbezierhandle.h",
        "handle/graphicsregularhandle.cpp",
        "handle/graphicsregularhandle.h",
        "item/schitem.cpp",
        "item/schitem.h",
        "item/igraphicsitemobserver.cpp",
        "item/igraphicsitemobserver.h",
        "item/igraphicsobservableitem.cpp",
        "item/igraphicsobservableitem.h",
        "item/graphicsarcitem.cpp",
        "item/graphicsarcitem.h",
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
        "item/graphicstextframeitem.cpp",
        "item/graphicstextframeitem.h",
        "item/graphicswireitem.cpp",
        "item/graphicswireitem.h",
        "propertyeditor/objectpropertyeditor.cpp",
        "propertyeditor/objectpropertyeditor.h",
        "propertyeditor/peneditorfactory.cpp",
        "propertyeditor/peneditorfactory.h",
        "propertyeditor/penpropertymanager.cpp",
        "propertyeditor/penpropertymanager.h",
        "propertyeditor/schitempropertyeditor.cpp",
        "propertyeditor/schitempropertyeditor.h",
        "propertyeditor/schitempropertymanager.cpp",
        "propertyeditor/schitempropertymanager.h",
        "scheditordocument.cpp",
        "scheditordocument.h",
        "snap/positionsnapper.cpp",
        "snap/positionsnapper.h",
        "tool/schtool.cpp",
        "tool/schtool.h",
        "tool/abstractgraphicsinserttool.cpp",
        "tool/abstractgraphicsinserttool.h",
        "tool/abstractgraphicsinteractivetool.cpp",
        "tool/abstractgraphicsinteractivetool.h",
        "tool/graphicsarctool.cpp",
        "tool/graphicsarctool.h",
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
        "palette.h",
        "palette.cpp",
        "scheditor.cpp",
        "scheditor.h",
        "scheditor.qrc",
        "scheditorconstants.h",
        "scheditorfactory.cpp",
        "scheditorfactory.h",
        "scheditorplugin.cpp",
        "scheditorplugin.h",
        "scheditorwidget.cpp",
        "scheditorwidget.h",
        "schscene.cpp",
        "schscene.h",
        "schview.cpp",
        "schview.h",
    ]
}
