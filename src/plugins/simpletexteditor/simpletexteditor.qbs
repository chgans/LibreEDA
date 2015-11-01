import qbs 1.0

LedaPlugin {
    name: "SimpleTextEditor"
    
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "opengl"] }
    Depends { name: "Utils" }

    files: [
        "simpletexteditor.cpp",
        "simpletexteditor.h",
        "simpletexteditorconstants.h",
        "simpletexteditorfactory.cpp",
        "simpletexteditorfactory.h",
        "simpletexteditorplugin.cpp",
        "simpletexteditorplugin.h",
    ]
}
