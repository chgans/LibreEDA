import qbs 1.0

LedaProduct {
    type: "application" // no Mac app bundle

    Group {
        fileTagsFilter: ["application"]
        qbs.install: true
        qbs.installDir: project.leda_app_path
    }

    cpp.includePaths: ["../../libs", "../../plugins"]
}
