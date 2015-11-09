import qbs 1.0
import LedaFunctions

Product {
    name: "Documentation"
    type: "qch"
    Depends { name: "Qt"; submodules: ["core"] }

    files: [
        "*.qdoc",
        "config/*.qdocconf",
        "leda.qmodel",
    ]
    Group {
        name: "main qdocconf file"
        files: "leda.qdocconf"
        fileTags: "qdocconf-main"
    }

    Qt.core.qdocQhpFileName: "leda.qhp"
    Qt.core.qdocEnvironment: [
        "LEDA_VERSION=" + project.leda_version,
        "SRCDIR=" + path,
        "QT_INSTALL_DOCS=" + Qt.core.docPath,
        "LEDA_VERSION_TAG=" + project.leda_version.replace(/\.|-/g, "")
    ]
    Group {
        fileTagsFilter: "qdoc-output"
        qbs.install: true
        qbs.installDir: project.leda_doc_path + "/html"
    }
}
