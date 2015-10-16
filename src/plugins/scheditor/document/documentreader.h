#ifndef DOCUMENTREADER_H
#define DOCUMENTREADER_H

#include <QByteArray>

class SchDocument;

class DocumentReader
{
public:
    DocumentReader(const QByteArray &data);

    SchDocument *read();

private:
    QByteArray m_data;
};

#endif // DOCUMENTREADER_H
