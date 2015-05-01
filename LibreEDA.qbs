import qbs 1.0

Project {
    minimumQbsVersion: "1.4"
    property string leda_version_major: '0'
    property string leda_version_minor: '0'
    property string leda_version_release: '0'
    property string leda_version: leda_version_major + '.' + leda_version_minor + '.' + leda_version_release
    property string leda_compat_version_major: '0'
    property string leda_compat_version_minor: '0'
    property string leda_compat_version_release: '0'
    property string leda_compat_version: leda_compat_version_major + '.' + leda_compat_version_minor + '.' + leda_compat_version_release
    property path leda_source_tree: path
    property string leda_app_path: "bin"
    property string leda_app_target: "libreeda"
    property string leda_library_path: "lib/leda"
    property string leda_plugin_path: leda_library_path + "/plugins"
    property string leda_data_path: "share/leda"
    property string leda_libexec_path: leda_app_path
    property string leda_doc_path: "share/doc/leda"
    property string leda_bin_path: leda_app_path
    property stringList generalDefines: [
        "LIBRE_EDA",
        'BASE_SAMPLE_DIR="' + sourceDirectory + '/data/samples"',
        'BASE_SETTINGS_DIR="' + sourceDirectory + '/data/settings"',
        // FIXME: use installRoot only for debug.
        'LEDA_PLUGIN_PATH="' +  qbs.installRoot + "/" + leda_plugin_path + '"'
        // "QT_NO_CAST_TO_ASCII",
        // "QT_NO_CAST_FROM_ASCII"
    ]
    property pathList additionalPlugins: []
    property pathList additionalLibs: []
    property pathList additionalTools: []
    property pathList additionalAutotests: []
    qbsSearchPaths: "qbs"

    references: [
        "doc/doc.qbs",
        "src/src.qbs"
    ]
}
