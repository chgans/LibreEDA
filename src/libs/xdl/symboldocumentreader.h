#ifndef SYMBOLPARSER_H
#define SYMBOLPARSER_H

#include <QString>

namespace xdl { namespace symbol {

class Document;
struct DocumentReaderPrivate;

class DocumentReader
{
public:
    DocumentReader();

    Document *parse(const QString &filename);

    QString errorString() const
    {
        return m_errorString;
    }

private:
    DocumentReaderPrivate *p;
    QString m_errorString;
};

}}

#endif // SYMBOLPARSER_H
