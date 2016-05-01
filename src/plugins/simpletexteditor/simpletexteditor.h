#ifndef SIMPLETEXTEDITOR_H
#define SIMPLETEXTEDITOR_H

#include "core/editormanager/ieditor.h"

class QTextEdit;
class QTextDocument;
class SimpleTextDocument;

class SimpleTextEditor : public IEditor
{
    Q_OBJECT
public:
    explicit SimpleTextEditor(QObject *parent = nullptr);

signals:

public slots:


    // IEditor interface
public:
    bool open(QString *errorString, const QString &fileName);
    IDocument *document() const;
    QIcon icon() const;
    QString displayName() const;
    void activate(QMainWindow *mainWindow);
    void desactivate(QMainWindow *mainWindow);

private:
    QTextEdit *m_textEdit;
    SimpleTextDocument *m_document;
};

#endif // SIMPLETEXTEDITOR_H
