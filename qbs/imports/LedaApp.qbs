import qbs 1.0

LedaProduct {
    type: "application" // no Mac app bundle
    installDir:  project.leda_app_path
    cpp.includePaths: ["../../libs", "../../plugins"]
}
