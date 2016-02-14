import qbs 1.0

LedaLibrary {
    name: "QtPropertyBrowser"
    cpp.defines: base.concat(["QTPROPERTYBROWSER_LIBRARY", ])

    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }

    files: [
        "qtbuttonpropertybrowser.cpp",
        "qtbuttonpropertybrowser.h",
        "qteditorfactory.cpp",
        "qteditorfactory.h",
        "qtgroupboxpropertybrowser.cpp",
        "qtgroupboxpropertybrowser.h",
        "qtpropertybrowser.cpp",
        "qtpropertybrowser.h",
        "qtpropertybrowser.qrc",
        "qtpropertybrowserutils.cpp",
        "qtpropertybrowserutils_p.h",
        "qtpropertymanager.cpp",
        "qtpropertymanager.h",
        "qttreepropertybrowser.cpp",
        "qttreepropertybrowser.h",
        "qtvariantproperty.cpp",
        "qtvariantproperty.h",
        "qtpropertybrowser_global.h"
    ]
}
