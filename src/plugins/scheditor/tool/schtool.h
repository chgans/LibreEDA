#ifndef GRAPHICSTOOL_H
#define GRAPHICSTOOL_H

#include <QObject>


class SchView;
class SchScene;
class SchEditorSettings;
class UndoCommand;

class QWidget;
class QAction;

/* TBD:
 *  - Actions (hooked with context menu)
 *    - Esc => cancel()
 *    - Tab => options
 *  - Use case: align tool has 6 actions
 *  - replace option dialog w/ option widget: allow to dosplay widget in a QDock or something
 */

class SchTool : public QObject
{
    Q_OBJECT
public:
    explicit SchTool(QObject *parent = nullptr);
    ~SchTool();

    SchView *view();
    SchScene *scene();
    void setView(SchView *view);

    void setToolGroup(const QString &group);
    QString toolGroup() const;
    void setAction(QAction *action);
    QAction *action() const;

signals:
    void finished();
    void taskCompleted(UndoCommand *command);

public slots:
    virtual void cancel();
    virtual void applySettings(const SchEditorSettings &settings);

private:
    SchView *m_view;
    QString m_toolGroup;
    QAction *m_action;
};

Q_DECLARE_METATYPE(SchTool *)

#endif // GRAPHICSTOOL_H
