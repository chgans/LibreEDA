import qbs

LedaApplication {
    type: "application"
    name: "Application"
    targetName: project.leda_app_target
    consoleApplication: qbs.debugInformation

    //Depends { name: "app_version_header" }
    Depends { name: "Qt"; submodules: ["core", "widgets", "network"] }
    Depends { name: "Utils" }
    Depends { name: "Core" }

    // TODO: use real plugins
    Depends { name: "PcbEditor" }
    Depends { name: "LogViewer" }
    Depends { name: "FilesystemNavigationView" }
    Depends { name: "SchEditor" }

    files : [
        "main.cpp",
        "mainwindow.cpp",
        "mainwindow.h"
    ]
}		
