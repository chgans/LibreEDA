#ifndef PCBEDITOR_H
#define PCBEDITOR_H

#include "core/editormanager/ieditor.h"

class PcbDocument;
class PcbEditorWidget;

class PcbEditor: public IEditor
{
    Q_OBJECT
public:
    PcbEditor(QObject *parent = nullptr);
    ~PcbEditor();

    // IEditor interface
public:
    bool open(QString *errorString, const QString &fileName);
    IDocument *document() const;
    void saveState(QSettings *settings) const;
    bool restoreState(QSettings *settings);
    QIcon icon() const;
    QString displayName() const;
    void activate(QMainWindow *mainWindow);
    void desactivate(QMainWindow *mainWindow);

private:
    PcbDocument *m_document = nullptr;
    PcbEditorWidget *pcbWidget() const;

};

#endif // PCBEDITOR_H

