#ifndef SYMBOLWRITER_H
#define SYMBOLWRITER_H

#include <QString>

namespace xdl { namespace symbol {

class Document;
class DocumentWriterPrivate;

class DocumentWriter
{
public:
    DocumentWriter();

    bool write(const QString &filename, const Document *symbol);

    QString errorString() const
    {
        return m_errorString;
    }

private:
    DocumentWriterPrivate *p;
    QString m_errorString;
};

}}
#endif // SYMBOLWRITER_H
