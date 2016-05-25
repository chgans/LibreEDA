import qbs

Project {
    name: "Plugins"
    references: [
        "filesystemnavigationview",
        "logviewer",
        "pcbeditor",
        "symboleditor",
        "simpletexteditor",
    ].concat(project.additionalPlugins)
}
