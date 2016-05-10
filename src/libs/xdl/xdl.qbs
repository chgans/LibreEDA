import qbs 1.0

LedaLibrary {
    name: "Xdl"
    cpp.defines: base.concat([
                                 "XDL_LIBRARY",
                             ])

    cpp.dynamicLibraries: [
        "expat"
    ]

    Depends { name: "Qt"; submodules: ["core", "gui"] }

    files: [
        "leda-xdl-symbol-pskel.cxx",
        "leda-xdl-symbol.make",
        "leda-xdl-symbol.map",
        "leda-xdl-symbol-pimpl.cpp",
        "leda-xdl-symbol-pimpl.h",
        "leda-xdl-symbol.xsd",
        "leda-xdl-symbol-pskel.hxx",
        "symboldocument.h",
        "symboldocument.cpp",
        "symboldocumentreader.cpp",
        "symboldocumentreader.h",
        "symboldocumentwriter.cpp",
        "symboldocumentwriter.h",
        "symbolitem.h",
        "xdl_global.h",
    ]
}
