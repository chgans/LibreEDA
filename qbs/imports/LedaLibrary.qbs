import qbs 1.0
import LedaFunctions

LedaProduct {
    type: "dynamiclibrary"
    installDir: project.leda_library_path
    targetName: LedaFunctions.qtLibraryName(qbs, name)
    destinationDirectory: project.leda_library_path

    // For #include <libname/header.h>, #include <pluginname/header.h>,
    // #include "foo/bar.h" #include "ui_header.h"
    cpp.includePaths: ["../../libs", "../../plugins", sourceDirectory, buildDirectory]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["../../libs", "../../plugins"]
    }
}
