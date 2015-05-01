import qbs 1.0

LedaPlugin {
    name: "FilesystemNavigationView"
    
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }
    Depends { name: "Utils" }

    files: [
        "filesystemnavigationplugin.cpp",
        "filesystemnavigationplugin.h",
        "filesystemnavigationviewfactory.cpp",
        "filesystemnavigationviewfactory.h",
        "filesystemnavigationwidget.cpp",
        "filesystemnavigationwidget.h",
    ]
}
