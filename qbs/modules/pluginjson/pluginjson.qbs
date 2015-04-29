import qbs 1.0
import qbs.TextFile
import qbs.FileInfo

Module {
    Depends { id: qtcore; name: "Qt.core" }

    additionalProductTypes: ["qt_plugin_metadata"]

    Rule {
        inputs: ["pluginJsonIn"]

        Artifact {
            fileTags: ["qt_plugin_metadata"]
            filePath: {
                var destdir = FileInfo.joinPaths(product.moduleProperty("Qt.core",
                                                         "generatedFilesDir"), input.fileName);
                return destdir.replace(/\.[^\.]*$/,'')
            }
        }

        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "prepare " + FileInfo.fileName(output.filePath);
            cmd.highlight = "codegen";
            cmd.pluginJsonReplacements = product.pluginJsonReplacements;
            cmd.plugin_depends = [];
            var deps = product.dependencies;
            for (var d in deps) {
                var depdeps = deps[d].dependencies;
                for (var dd in depdeps) {
                    if (depdeps[dd].name == 'pluginjson') {
                        cmd.plugin_depends.push(deps[d].name);
                        break;
                    }
                }
            }
            cmd.plugin_recommends = product.pluginRecommends
            cmd.plugin_test_depends = product.pluginTestDepends

            cmd.sourceCode = function() {
                var i;
                var vars = pluginJsonReplacements || {};
                var inf = new TextFile(input.filePath);
                var all = inf.readAll();
                // replace quoted quotes
                all = all.replace(/\\\"/g, '"');
                // replace config vars
                vars['LEDA_VERSION'] = project.leda_version;
                vars['LEDA_COMPAT_VERSION'] = project.leda_compat_version;
                vars['LEDA_VERSION_MAJOR'] = project.leda_version_major;
                vars['LEDA_VERSION_MINOR'] = project.leda_version_minor;
                vars['LEDA_VERSION_RELEASE'] = project.leda_version_release;
                var deplist = [];
                for (i in plugin_depends) {
                    deplist.push("        { \"Name\" : \"" + plugin_depends[i] + "\", \"Version\" : \"" + project.qtcreator_version + "\" }");
                }
                for (i in plugin_recommends) {
                    deplist.push("        { \"Name\" : \"" + plugin_recommends[i] + "\", \"Version\" : \"" + project.qtcreator_version + "\", \"Type\" : \"optional\" }");
                }
                for (i in plugin_test_depends) {
                    deplist.push("        { \"Name\" : \"" + plugin_test_depends[i] + "\", \"Version\" : \"" + project.qtcreator_version + "\", \"Type\" : \"test\" }");
                }
                deplist = deplist.join(",\n")
                vars['dependencyList'] = "\"Dependencies\" : [\n" + deplist + "\n    ]";
                for (i in vars) {
                    all = all.replace(new RegExp('\\\$\\\$' + i + '(?!\w)', 'g'), vars[i]);
                }
                var file = new TextFile(output.filePath, TextFile.WriteOnly);
                file.truncate();
                file.write(all);
                file.close();
            }
            return cmd;
        }
    }
}

