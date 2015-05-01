import qbs 1.0
import qbs.FileInfo
import LedaFunctions

LedaProduct {
    type: ["dynamiclibrary", "pluginSpec"]
    installDir: project.leda_plugin_path

    property var pluginJsonReplacements
    property var pluginRecommends: []
    property var pluginTestDepends: []

    property string minimumQtVersion: "5.4.0"
    condition: LedaFunctions.versionIsAtLeast(Qt.core.version, minimumQtVersion)

    targetName: LedaFunctions.qtLibraryName(qbs, name)
    destinationDirectory: project.leda_plugin_path

    Depends { name: "Core" }
    Depends { name: "pluginjson" }
    Depends {
        condition: project.testsEnabled
        name: "Qt.test"
    }

    cpp.defines: base.concat([name.toUpperCase() + "_LIBRARY"])
    cpp.installNamePrefix: "@rpath/PlugIns/"
    cpp.rpaths: qbs.targetOS.contains("osx")
                ? ["@loader_path/..", "@loader_path/", "@executable_path/.."]
                : ["$ORIGIN", "$ORIGIN/.."]
    cpp.linkerFlags: {
        var flags = base;
        if (qbs.buildVariant == "debug" && qbs.toolchain.contains("msvc"))
            flags.push("/INCREMENTAL:NO"); // Speed up startup time when debugging with cdb
        return flags;
    }

    Group {
        name: "PluginMetaData"
        files: [ product.name + ".json.in" ]
        fileTags: ["pluginJsonIn"]
    }

    Group {
        name: "MimeTypes"
        files: [ "*.mimetypes.xml" ]
    }

    // For #include <libname/header.h>, #include <pluginname/header.h>,
    // #include "foo/bar.h" #include "ui_header.h"
    cpp.includePaths: ["../../libs", "../../plugins", sourceDirectory, buildDirectory]

    Export {
        //Depends { name: "ExtensionSystem" }
        Depends { name: "cpp" }
        //cpp.includePaths: [pluginIncludeBase]
    }
}
