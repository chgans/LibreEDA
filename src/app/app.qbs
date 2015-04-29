import qbs

LedaProduct {
    type: ["application"]
    name: "LibreEDA"
    targetName: project.leda_app_target
    consoleApplication: qbs.debugInformation

    //Depends { name: "app_version_header" }
    Depends { name: "Qt"; submodules: ["core", "widgets", "network"] }
    Depends { name: "Utils" }
    Depends { name: "Core" }

    // TODO: use real plugins
    Depends { name: "SchEditor" }
    Depends { name: "PcbEditor" }
    Depends { name: "LogViewer" }
    Depends { name: "FilesystemNavigationView" }

    files : [
        "main.cpp",
        "mainwindow.cpp",
        "mainwindow.h"
    ]
}		
