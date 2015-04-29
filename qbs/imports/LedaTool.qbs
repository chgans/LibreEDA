import qbs 1.0

LedaProduct {
    type: "application" // no Mac app bundle
    installDir:  project.leda_libexec_path

    cpp.rpaths: qbs.targetOS.contains("osx")
            ? ["@executable_path/../" + project.leda_library_path]
            : ["$ORIGIN/../" + project.leda_library_path]
}
