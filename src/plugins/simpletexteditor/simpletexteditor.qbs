import qbs 1.0

LedaPlugin {
    name: "SimpleTextEditor"
    
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "opengl"] }
    Depends { name: "Core" }
    //Depends { name: "Utils" }

    files: [
        "simpletextdocument.cpp",
        "simpletextdocument.h",
        "simpletexteditor.cpp",
        "simpletexteditor.h",
        "simpletexteditor.qrc",
        "simpletexteditorconstants.h",
        "simpletexteditorfactory.cpp",
        "simpletexteditorfactory.h",
        "simpletexteditorplugin.cpp",
        "simpletexteditorplugin.h",
    ]
}
