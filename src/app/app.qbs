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

    files : [
        "main.cpp",
        "mainwindow.cpp",
        "mainwindow.h"
    ]
}		
