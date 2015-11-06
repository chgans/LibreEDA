#include "simpletextdocument.h"

#include <QTextDocument>
#include <QFile>

SimpleTextDocument::SimpleTextDocument():
    m_textDocument(new QTextDocument)
{

}

bool SimpleTextDocument::load(QString *errorString, const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        *errorString = file.errorString();
        return false;
    }
    m_textDocument->setPlainText(file.readAll());
    return true;
}

QTextDocument *SimpleTextDocument::textDocument()
{
    return m_textDocument;
}

bool SimpleTextDocument::save(QString *errorString, const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        *errorString = file.errorString();
        return false;
    }
    const QByteArray bytes = m_textDocument->toPlainText().toLatin1();
    if (file.write(bytes.constData(), bytes.size()) < 0) {
        *errorString = file.errorString();
        return false;
    }
    return true;
}

