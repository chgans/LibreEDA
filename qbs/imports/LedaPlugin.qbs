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

    //Depends { name: "ExtensionSystem" }
    //Depends { name: "pluginjson" }
    Depends {
        condition: project.testsEnabled
        name: "Qt.test"
    }

    cpp.defines: base.concat([name.toUpperCase() + "_LIBRARY"])

    Group {
        name: "PluginMetaData"
        files: [ product.name + ".json.in" ]
        fileTags: ["pluginJsonIn"]
    }

    Group {
        name: "MimeTypes"
        files: [ "*.mimetypes.xml" ]
    }

    Export {
        //Depends { name: "ExtensionSystem" }
        Depends { name: "cpp" }
        //cpp.includePaths: [pluginIncludeBase]
    }
}
