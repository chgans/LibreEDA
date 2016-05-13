#ifndef SYMBOLWRITER_H
#define SYMBOLWRITER_H

#include <QString>

namespace xdl {
namespace symbol {

class Symbol;
struct WriterPrivate;

class Writer
{
public:
    Writer();
    ~Writer();

    bool write(const QString &filename, const Symbol *symbol);
    QString errorString() const;

private:
    WriterPrivate *p;
    QString m_errorString;
};

}
}
#endif // SYMBOLWRITER_H
