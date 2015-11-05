import qbs 1.0

// TODO: rename to LoggerOutputPane
LedaPlugin {
    name: "LogViewer"

    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }
    Depends { name: "Utils" }

    files: [
        "logmessage.cpp",
        "logmessage.h",
        "logmodel.cpp",
        "logmodel.h",
        "logsortfilterproxymodel.cpp",
        "logsortfilterproxymodel.h",
        "logviewer.cpp",
        "logviewer_global.h",
        "logviewer.h",
        "logviewerplugin.cpp",
        "logviewerplugin.h",
        "logwidget.cpp",
        "logwidget.h",
        "logwidget.ui",
    ]
}
