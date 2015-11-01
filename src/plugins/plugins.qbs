import qbs

Project {
    name: "Plugins"
    references: [
        "filesystemnavigationview",
        "logviewer",
        "pcbeditor",
        "scheditor",
        "simpletexteditor",
    ].concat(project.additionalPlugins)
}
