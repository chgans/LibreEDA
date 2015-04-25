#ifndef WIREOPERATIONWIDGET_H
#define WIREOPERATIONWIDGET_H

#include <QWidget>

class CoordinateWidget;
class QPushButton;
class GraphicsWireTool;

class WireOperationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WireOperationWidget(QWidget *parent = 0);

    void setTool(GraphicsWireTool *tool);

signals:
    void addPointRequested(const QPointF &point);
    void finishRequested();
    void closeWireRequested();
    void wipeRequested();

public slots:
    void setCurrentLocation(const QPointF &pos);

private:
    GraphicsWireTool *m_tool;
    CoordinateWidget *m_coordinateWidget;
    QPushButton *m_undoButton;
    QPushButton *m_finishButton;
    QPushButton *m_closeButton;
    QPushButton *m_wipeButton;

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    bool focusNextPrevChild(bool next);
};

#endif // WIREOPERATIONWIDGET_H
