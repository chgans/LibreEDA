import qbs 1.0

Project {
    minimumQbsVersion: "1.4"
    property bool withAutotests: qbs.buildVariant === "debug"
    property string leda_version_major: '3'
    property string leda_version_minor: '4'
    property string leda_version_release: '81'
    property string leda_version: leda_version_major + '.' + leda_version_minor + '.' + leda_version_release
    property string leda_compat_version_major: '3'
    property string leda_compat_version_minor: '4'
    property string leda_compat_version_release: '81'
    property string leda_compat_version: leda_compat_version_major + '.' + leda_compat_version_minor + '.' + leda_compat_version_release
    property path leda_source_tree: path
    property string leda_app_path: qbs.targetOS.contains("osx") ? "" : "bin"
    property string leda_app_target: qbs.targetOS.contains("osx") ? "Libre EDA" : "LibreEDA"
    property pathList additionalPlugins: []
    property pathList additionalLibs: []
    property pathList additionalTools: []
    property pathList additionalAutotests: []
    property string sharedSourcesDir: path + "/src/shared"
    property string libDirName: "lib"
    property string leda_library_path: {
        if (qbs.targetOS.contains("osx"))
            return leda_app_target + ".app/Contents/Frameworks"
        else if (qbs.targetOS.contains("windows"))
            return leda_app_path
        else
            return libDirName + "/leda"
    }
    property string leda_plugin_path: {
        if (qbs.targetOS.contains("osx"))
            return leda_app_target + ".app/Contents/PlugIns"
        else if (qbs.targetOS.contains("windows"))
            return libDirName + "/leda/plugins"
        else
            return leda_library_path + "/plugins"
    }
    property string leda_data_path: qbs.targetOS.contains("osx")
                                    ? leda_app_target + ".app/Contents/Resources"
                                    : "share/leda"
    property string leda_libexec_path: qbs.targetOS.contains("osx")
                                       ? leda_data_path
                                       : leda_app_path
    property string leda_doc_path: qbs.targetOS.contains("osx")
                                   ? leda_data_path + "/doc"
                                   : "share/doc/leda"
    property string leda_bin_path: qbs.targetOS.contains("osx")
                                   ? leda_app_target + ".app/Contents/MacOS"
                                   : leda_app_path
    property bool testsEnabled: qbs.getEnv("TEST") || qbs.buildVariant === "debug"
    property stringList generalDefines: [
        "LIBRE_EDA",
	'BASE_SAMPLE_DIR="' + sourceDirectory + '/data/samples"',
	'BASE_SETTINGS_DIR="' + sourceDirectory + '/data/settings"',
	'LEDA_LIBRARY_BASENAME="' + libDirName + '"'
	// "QT_NO_CAST_TO_ASCII",
        // "QT_NO_CAST_FROM_ASCII"
    ].concat(testsEnabled ? ["WITH_TESTS"] : [])
    qbsSearchPaths: "qbs"

    references: [
        "src/src.qbs"
    ]

    //AutotestRunner {}
}
