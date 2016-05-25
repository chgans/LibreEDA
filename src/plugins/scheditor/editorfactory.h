#pragma once

#include "core/editormanager/ieditorfactory.h"

namespace SymbolEditor
{
    class Editor;

    class EditorFactory : public IEditorFactory
    {
        Q_OBJECT
    public:
        explicit EditorFactory(QObject *parent = nullptr);

    signals:

    public slots:
        void applySettings();

    private:
        QList<Editor *> m_editors;

        // IEditorFactory interface
    public:
        virtual IEditor *createEditor();
    };
}
