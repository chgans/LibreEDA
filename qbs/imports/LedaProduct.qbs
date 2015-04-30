import qbs 1.0
import LedaFunctions

Product {
    property string installDir

    Depends { name: "cpp" }
    cpp.defines: project.generalDefines
    cpp.cxxLanguageVersion: "c++11"

    Depends { name: "Qt.core" }

    Properties {
        condition: qbs.targetOS.contains("darwin")
        cpp.rpaths: base.concat(["@loader_path/../" + project.leda_library_path])
    }

    Properties {
        condition: qbs.targetOS.contains("linux")
        cpp.rpaths: base.concat(["$ORIGIN/../" + project.leda_library_path])
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: installDir
    }
}
