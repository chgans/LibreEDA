#include "documentreader.h"

#include <QXmlInputSource>
#include <QXmlContentHandler>
#include <QXmlLocator>

class ContentHandler: public QXmlContentHandler
{
public:
    ContentHandler() {}
    virtual ~ContentHandler() {}

    // QXmlContentHandler interface
public:
    virtual void setDocumentLocator(QXmlLocator *locator);
    virtual bool startDocument();
    virtual bool endDocument();
    virtual bool startPrefixMapping(const QString &prefix, const QString &uri);
    virtual bool endPrefixMapping(const QString &prefix);
    virtual bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    virtual bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    virtual bool characters(const QString &ch);
    virtual bool ignorableWhitespace(const QString &ch);
    virtual bool processingInstruction(const QString &target, const QString &data);
    virtual bool skippedEntity(const QString &name);
    virtual QString errorString() const;

private:
    enum State {
        NotStarted,
        InDocument,
        InObject,
        InProperty
    };
    State m_state = NotStarted;
    QXmlLocator *m_xmlLocator = nullptr;
};

void ContentHandler::setDocumentLocator(QXmlLocator *locator)
{
    m_xmlLocator = locator;
}

bool ContentHandler::startDocument()
{
    m_state = InDocument;
    return true;
}

bool ContentHandler::endDocument()
{
    m_state = NotStarted;
    return true;
}

bool ContentHandler::startPrefixMapping(const QString &prefix, const QString &uri)
{
    Q_UNUSED(prefix);
    Q_UNUSED(uri);
    return true;
}

bool ContentHandler::endPrefixMapping(const QString &prefix)
{
    Q_UNUSED(prefix);
    return true;
}

bool ContentHandler::startElement(const QString &namespaceURI, const QString &localName,
                                  const QString &qName, const QXmlAttributes &atts)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(qName);
    switch(m_state) {
    case ContentHandler::NotStarted:
        if (localName != "document") {
            return false;
        }
        if (atts.index("type") < 0) {
            return false;
        }
        break;
    case ContentHandler::InDocument:
        break;
    case ContentHandler::InObject:
        break;
    case ContentHandler::InProperty:
        break;
    }
    return false;
}

bool ContentHandler::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(localName);
    Q_UNUSED(qName);
    return false;
}

bool ContentHandler::characters(const QString &ch)
{
    Q_UNUSED(ch);
    return false;
}

bool ContentHandler::ignorableWhitespace(const QString &ch)
{
    Q_UNUSED(ch);
    return true;
}

bool ContentHandler::processingInstruction(const QString &target, const QString &data)
{
    Q_UNUSED(target);
    Q_UNUSED(data);
    return true;
}

bool ContentHandler::skippedEntity(const QString &name)
{
    Q_UNUSED(name);
    return true;
}

QString ContentHandler::errorString() const
{
    return QString();
}



DocumentReader::DocumentReader(const QByteArray &data):
    m_data(data)
{

}

/*
 *  <document type="leda.schematics.sym">
 *    <object id="" type="">
 *     <property id="" type="">
 *      ...
 *     </property>
 *    </object>
 *  </document>
 */

SchDocument *DocumentReader::read()
{
    return nullptr;
}
