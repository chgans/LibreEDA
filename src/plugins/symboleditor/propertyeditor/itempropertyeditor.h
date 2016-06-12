#pragma once

#include "document.h"

#include <QWidget>

class QtAbstractPropertyBrowser;

namespace SymbolEditor
{

    class UndoCommand;
    class PropertyManager;

    class ItemPropertyEditor : public QWidget
    {
        Q_OBJECT
    public:
        explicit ItemPropertyEditor(QWidget *parent = nullptr);
        virtual ~ItemPropertyEditor();

        const Document::Item *item() const;

    signals:
        void commandRequested(UndoCommand *command);

    public slots:
        void setItem(const Document::Item *item);
        void updateProperty(quint64 id, const QVariant &value);
        void clear();

    private slots:
        void onValueChanged(quint64 id, const QVariant &value);

    private:
        void load();
        bool m_updatingProperties;
        QtAbstractPropertyBrowser *m_browser;
        PropertyManager *m_manager;
        const Document::Item *m_item;

        void addCoordinate(quint64 id);
        void addLength(quint64 id);
        void addAngle(quint64 id);
        void addPercentage(quint64 id);
        void addColor(quint64 id);
        void addBistate(quint64 id);
        void addText(quint64 id);
        void addLineStyle(quint64 id);
        void addLineWidth(quint64 id);
        void addFillStyle(quint64 id);
    };

}
