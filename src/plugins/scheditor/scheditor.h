#ifndef GRAPHICSEDITOR_H
#define GRAPHICSEDITOR_H

#include "core/editormanager/ieditor.h"

class SchDocument;
class SchEditorWidget;

class SchEditor : public IEditor
{
    Q_OBJECT
public:
    explicit SchEditor(QObject *parent = 0);

signals:

public slots:


    // IEditor interface
public:
    bool open(QString *errorString, const QString &fileName);
    IDocument *document() const;
    QIcon icon() const;
    QString displayName() const;

private:
    SchDocument *m_document = nullptr;
    SchEditorWidget *schWidget() const;
};

#endif // GRAPHICSEDITOR_H
