#ifndef GRAPHICSTOOL_H
#define GRAPHICSTOOL_H

#include <QObject>


class SchView;
class SchScene;

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
    explicit SchTool(QObject *parent = 0);
    ~SchTool();

    SchView *view();
    SchScene *scene();
    virtual void setView(SchView *view);

    void setToolGroup(const QString &group);
    QString toolGroup() const;
    void setAction(QAction *action);
    QAction *action() const;

    virtual void activate(const QAction *which, SchView *view) = 0;
    virtual void desactivate(const QAction *which, SchView *view) = 0;

signals:
    void finished();

public slots:
    virtual void cancel();

private:
    SchView *m_view;
    QString m_toolGroup;
    QAction *m_action;
};

Q_DECLARE_METATYPE(SchTool*)

#endif // GRAPHICSTOOL_H
