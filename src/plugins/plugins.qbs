import qbs

Project {
    name: "Plugins"
    references: [
        "filesystemnavigationview",
        "logviewer",
        "pcbeditor",
        "scheditor",
    ].concat(project.additionalPlugins)
}
