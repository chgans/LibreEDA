#ifndef SYMBOLWRITER_H
#define SYMBOLWRITER_H

#include <QString>

namespace xdl { namespace symbol {

class Document;
class SymbolWriterPrivate;

class SymbolWriter
{
public:
    SymbolWriter();

    bool write(const QString &filename, const Document *symbol);

    QString errorString() const
    {
        return m_errorString;
    }

private:
    SymbolWriterPrivate *p;
    QString m_errorString;
};

}}
#endif // SYMBOLWRITER_H
