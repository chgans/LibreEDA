import qbs 1.0
import LedaFunctions

LedaProduct {
    type: "dynamiclibrary"
    installDir: project.leda_library_path
    Depends {
        condition: project.testsEnabled
        name: "Qt.test"
    }

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

    // For #include <lib/header.h> and #include "ui_header.h"
    // FIXME: use LedaPlugin for plugins and move libs into their own dir
    cpp.includePaths: [".", "..", "../..", buildDirectory]

    Export {
        Depends { name: "cpp" }
        //cpp.includePaths: [libIncludeBase]
	//cpp.linkerFlags: "-L" + project.buildDirectory + "/lib/leda -l" + targetName
    }
}
