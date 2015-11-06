import qbs 1.0
import LedaFunctions

Product {
    property string installDir

    Depends { name: "cpp" }
    cpp.defines: project.generalDefines
    cpp.cxxLanguageVersion: "c++11"
    cpp.linkerFlags: {
        var flags = [];
        if (qbs.buildVariant == "release" && (qbs.toolchain.contains("gcc") || qbs.toolchain.contains("mingw")))
            flags.push("-Wl,-s");
        return flags;
    }

    Depends { name: "Qt.core" }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: installDir
    }
}
