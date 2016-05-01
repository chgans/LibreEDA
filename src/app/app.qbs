import qbs

LedaProduct {
    type: "application"
    name: project.leda_app_target
    targetName: project.leda_app_target
    version: project.leda_version
    consoleApplication: qbs.debugInformation
    installDir: project.leda_app_path

    Depends { name: "Qt"; submodules: ["core", "widgets", "network", "printsupport"] }
    Depends { name: "Utils" }
    Depends { name: "Core" }

    cpp.rpaths: ["$ORIGIN/../lib/leda"]

    files : [
        "libreeda.qrc",
        "main.cpp",
        "mainwindow.cpp",
        "mainwindow.h",
    ]
}
