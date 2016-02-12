import qbs 1.0

LedaLibrary {
    name: "Utils"
    cpp.defines: base.concat([
                                 "UTILS_LIBRARY",
                             ])

    Depends { name: "Qt"; submodules: ["core", "gui", "widgets"] }

    files: [
        "utils.cpp",
        "utils.h",
        "utils_global.h",
        "widgets/colorpreview.cpp",
        "widgets/colorpreview.h",
        "widgets/colorwheel.cpp",
        "widgets/colorwheel.h",
        "widgets/colorwidget.cpp",
        "widgets/colorwidget.h",
        "widgets/gradientslider.cpp",
        "widgets/gradientslider.h",
        "widgets/hueslider.cpp",
        "widgets/hueslider.h",
        "widgets/paintborder.h",
        "widgets/pencapstylecombobox.cpp",
        "widgets/pencapstylecombobox.h",
        "widgets/pencolorcombobox.cpp",
        "widgets/pencolorcombobox.h",
        "widgets/penjoinstylecombobox.cpp",
        "widgets/penjoinstylecombobox.h",
        "widgets/pensettingswidget.cpp",
        "widgets/pensettingswidget.h",
        "widgets/penstylecombobox.cpp",
        "widgets/penstylecombobox.h",
        "widgets/penwidthcombobox.cpp",
        "widgets/penwidthcombobox.h",
    ]
}
