import qbs 1.0

LedaLibrary {
    name: "Core"
    cpp.defines: base.concat([
                                 "CORE_LIBRARY",
                             ])

    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }

    files: [
        "abstracteditor.cpp",
        "abstracteditor.h",
        "constants.h",
        "core.cpp",
        "core.h",
        "core_global.h",
        "extension/extension.h",
        "extension/iplugin.cpp",
        "extension/iplugin.h",
        "extension/plugincollection.cpp",
        "extension/plugincollection.h",
        "extension/pluginmanager.cpp",
        "extension/pluginmanager.h",
        "extension/pluginspec.cpp",
        "extension/pluginspec.h",
        "json.cpp",
        "json.h",
        "actionmanager/actionmanager.cpp",
        "actionmanager/actionmanager.h",
        "editormanager/documentmanager.cpp",
        "editormanager/documentmanager.h",
        "editormanager/editormanager.cpp",
        "editormanager/editormanager.h",
        "editormanager/editorview.cpp",
        "editormanager/editorview.h",
        "editormanager/idocument.cpp",
        "editormanager/idocument.h",
        "editormanager/ieditor.cpp",
        "editormanager/ieditorfactory.cpp",
        "editormanager/ieditorfactory.h",
        "editormanager/ieditor.h",
        "navigationview/inavigationviewfactory.cpp",
        "navigationview/inavigationviewfactory.h",
        "navigationview/navigationdockwidget.cpp",
        "navigationview/navigationdockwidget.h",
        "outputpane/ioutputpanefactory.cpp",
        "outputpane/ioutputpanefactory.h",
        "outputpane/outputdockwidget.cpp",
        "outputpane/outputdockwidget.h",
    ]
}
