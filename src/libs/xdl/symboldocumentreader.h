#ifndef SYMBOLPARSER_H
#define SYMBOLPARSER_H

#include <QString>

namespace xdl { namespace symbol {

class Document;
struct SymbolReaderPrivate;

class SymbolReader
{
public:
    SymbolReader();

    Document *parse(const QString &filename);

    QString errorString() const
    {
        return m_errorString;
    }

private:
    SymbolReaderPrivate *p;
    QString m_errorString;
};

}}

#endif // SYMBOLPARSER_H
