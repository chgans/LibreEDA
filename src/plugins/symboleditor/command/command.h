#pragma once

#include <QUndoCommand>
#include <QPen>
#include <QPointF>
#include <QFont>

#include "xdl/symbol.h"

namespace SymbolEditor
{

    class Document;

    class UndoCommand: public QUndoCommand
    {
    public:
        UndoCommand(UndoCommand *parent = nullptr);

        void setDocument(Document *document);
        Document *document() const;

    protected:
        void warnItemNotFound(const QString command, quint64 id);

    private:
        Document *m_document;
    };

}
