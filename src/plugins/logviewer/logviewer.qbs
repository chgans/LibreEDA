import qbs 1.0

// TODO: rename to LoggerOutputPane
LedaPlugin {
    name: "LogViewer"

    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }
    //Depends { name: "Utils" }
    Depends { name: "Core" }

    files: [
        "loggingoutputpanefactory.cpp",
        "loggingoutputpanefactory.h",
        "logmessage.cpp",
        "logmessage.h",
        "logmodel.cpp",
        "logmodel.h",
        "logsortfilterproxymodel.cpp",
        "logsortfilterproxymodel.h",
        "logviewer_global.h",
        "logviewerplugin.cpp",
        "logviewerplugin.h",
        "logwidget.cpp",
        "logwidget.h",
        "logwidget.ui",
    ]
}
