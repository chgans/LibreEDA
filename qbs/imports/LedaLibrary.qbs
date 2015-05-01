import qbs 1.0
import LedaFunctions

LedaProduct {
    type: "dynamiclibrary"
    installDir: project.leda_library_path
    targetName: LedaFunctions.qtLibraryName(qbs, name)
    destinationDirectory: project.leda_library_path

    cpp.linkerFlags: {
        var flags = base;
        if (qbs.buildVariant == "debug" && qbs.toolchain.contains("msvc"))
            flags.push("/INCREMENTAL:NO"); // Speed up startup time when debugging with cdb
        return flags;
    }
    cpp.installNamePrefix: "@rpath/Frameworks/"
    cpp.rpaths: qbs.targetOS.contains("osx")
            ? ["@loader_path/..", "@executable_path/.."]
            : ["$ORIGIN", "$ORIGIN/.."]

    // For #include <libname/header.h>, #include <pluginname/header.h>,
    // #include "foo/bar.h" #include "ui_header.h"
    cpp.includePaths: ["../../libs", sourceDirectory, buildDirectory]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: ["../../libs", "../../plugins"]
    }
}
