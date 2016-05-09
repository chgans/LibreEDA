#ifndef GRAPHICSEDITOR_H
#define GRAPHICSEDITOR_H

#include "core/editormanager/ieditor.h"

class SchEditorDocument;
class SchEditorWidget;

class SchEditor : public IEditor
{
    Q_OBJECT
public:
    explicit SchEditor(QObject *parent = nullptr);
    ~SchEditor();

signals:

public slots:
    void loadSettings();

    // IEditor interface
public:
    bool open(QString *errorString, const QString &fileName);
    IDocument *document() const;
    QIcon icon() const;
    QString displayName() const;
    void activate(QMainWindow *mainWindow);
    void desactivate(QMainWindow *mainWindow);

private:
    SchEditorDocument *m_document = nullptr;
    SchEditorWidget *schWidget() const;

};

#endif // GRAPHICSEDITOR_H
