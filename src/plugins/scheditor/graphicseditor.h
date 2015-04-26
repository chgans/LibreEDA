#ifndef GRAPHICSEDITOR_H
#define GRAPHICSEDITOR_H

#include "core/editormanager/ieditor.h"

class GraphicsDocument;
class GraphicsEditorWidget;

class GraphicsEditor : public IEditor
{
    Q_OBJECT
public:
    explicit GraphicsEditor(QObject *parent = 0);

signals:

public slots:


    // IEditor interface
public:
    bool open(QString *errorString, const QString &fileName);
    IDocument *document() const;

private:
    GraphicsDocument *m_document = nullptr;
    GraphicsEditorWidget *schWidget() const;
};

#endif // GRAPHICSEDITOR_H
