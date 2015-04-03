#ifndef GRAPHICSTOOL_H
#define GRAPHICSTOOL_H

#include <QObject>


class GraphicsView;
class GraphicsScene;

class QWidget;
class QAction;

/* TBD:
 *  - Actions (hooked with context menu)
 *    - Esc => cancel()
 *    - Tab => options
 *  - Use case: align tool has 6 actions
 *  - replace option dialog w/ option widget: allow to dosplay widget in a QDock or something
 */

class GraphicsTool : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsTool(QObject *parent = 0);
    ~GraphicsTool();

    GraphicsView *view();
    GraphicsScene *scene();
    virtual void setView(GraphicsView *view);

    virtual QWidget *optionWidget() = 0;
    virtual QWidget *taskWidget() = 0;

    virtual QString toolGroup() const = 0;
    virtual QAction *action() const = 0;

    virtual void activate(const QAction *which, GraphicsView *view) = 0;
    virtual void desactivate(const QAction *which, GraphicsView *view) = 0;

signals:
    void finished();

public slots:
    virtual void cancel();

private:
    GraphicsView *m_view;
};

Q_DECLARE_METATYPE(GraphicsTool*)

#endif // GRAPHICSTOOL_H
