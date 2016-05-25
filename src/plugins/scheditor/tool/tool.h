#ifndef GRAPHICSTOOL_H
#define GRAPHICSTOOL_H

#include <QObject>

class QWidget;
class QAction;

namespace SymbolEditor
{

    class View;
    class Scene;
    class Settings;
    class UndoCommand;

    /* TBD:
     *  - Actions (hooked with context menu)
     *    - Esc => cancel()
     *    - Tab => options
     *  - Use case: align tool has 6 actions
     *  - replace option dialog w/ option widget: allow to dosplay widget in a QDock or something
     */

    class Tool : public QObject
    {
        Q_OBJECT
    public:
        explicit Tool(QObject *parent = nullptr);
        ~Tool();

        View *view();
        Scene *scene();
        void setView(View *view);

        void setToolGroup(const QString &group);
        QString toolGroup() const;
        void setAction(QAction *action);
        QAction *action() const;

    signals:
        void finished();
        void taskCompleted(UndoCommand *command);

    public slots:
        virtual void cancel();
        virtual void applySettings(const Settings &settings);

    private:
        View *m_view;
        QString m_toolGroup;
        QAction *m_action;
    };

}

Q_DECLARE_METATYPE(SymbolEditor::Tool *)

#endif // GRAPHICSTOOL_H
