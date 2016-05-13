#ifndef SYMBOLPARSER_H
#define SYMBOLPARSER_H

#include <QString>

namespace xdl {
namespace symbol {

class Symbol;
struct ReaderPrivate;

class Reader
{
public:
    Reader();

    Symbol *read(const QString &filename);
    QString errorString() const;

private:
    ReaderPrivate *p;
    QString m_errorString;
};

}
}

#endif // SYMBOLPARSER_H
