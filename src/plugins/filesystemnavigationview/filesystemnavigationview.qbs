import qbs 1.0

LedaLibrary {
    name: "FilesystemNavigationView"
    
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }
    Depends { name: "Core" }
    Depends { name: "Utils" }

    files: [
        "filesystemnavigationviewfactory.cpp",
        "filesystemnavigationviewfactory.h",
        "filesystemnavigationwidget.cpp",
        "filesystemnavigationwidget.h"
    ]
}
