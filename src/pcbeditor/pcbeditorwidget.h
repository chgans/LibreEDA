#ifndef PCBEDITORWIDGET_H
#define PCBEDITORWIDGET_H

#include "core/abstracteditor.h"

class QMainWindow;
class QAction;
class QMenu;
class QToolButton;

class MainView;
class Scene;
class LayerTabBar;
class PcbPalette;
class PcbPaletteManager;
class DesignLayerManager;
class InsightDockWidget;

class PcbEditorWidget : public AbstractEditor
{
    Q_OBJECT
public:
    explicit PcbEditorWidget(QWidget *parent = 0);
    ~PcbEditorWidget();

    MainView *graphicsView() { return m_view; }

    void setScene(Scene *scene);

    void activate(QMainWindow *window);
    void desactivate(QMainWindow *window);

    void readSettings(QSettings &settings);
    void writeSettings(QSettings &settings);

    QString type() const;

signals:

public slots:

protected slots:
    void showBoardInsightPopUpMenu();

protected:
    void wheelEvent(QWheelEvent *event);

private:
    PcbPaletteManager *m_paletteManager;
    DesignLayerManager *m_layerManager;
    MainView *m_view;
    LayerTabBar *m_layerTabBar;
    InsightDockWidget *m_insightDockWidget;
    QToolButton *m_snapButton;
    QToolButton *m_maskButton;
    QToolButton *m_clearButton;

    void createActions();
    void populateFakeData();

    Scene *scene() const;

    /** 2D Board Insight System **/
private:
    void createBoardInsightMenu();
    QAction *m_toggleHeadsUpDisplayAction; // Shift+H
    QAction *m_toggleHeadsUpTrackingAction; // Shift+G
    QAction *m_resetHeadsUpDeltaOriginAction; // Ins
    QAction *m_toggleHeadsUpDeltaOriginAction; // Shit+D
    QAction *m_toggleInsightLensAction; // Shift+M
    QAction *m_shiftInsightLensToMouseAction; // Shift+Ctrl+N
    QAction *m_toggleInsightLensShapeAction; // Shift+B
    QAction *m_toggleInsightLensTrackingAction; // Shift+N
    QAction *m_toggleInsightLensAutoZoomAction; // Shift+Ctrl+M
    QAction *m_toggleInsightLensSingleLayerModeAction; // Shift+Ctrl+S
    QAction *m_showBoardInsightPopUpMenuAction; // F2
    QMenu   *m_boardInsightPopUpMenu;

    QAction *m_insightLensZoomInAction; // Alt+WheelUp
    QAction *m_insightLensZoomOutAction; // Alt+WheelDown

    QAction *m_showInsightSystemViolationAction; // Shift+X
    QAction *m_showInsightSystemOtherAction; // Shift+V
    QAction *m_cycleSingleLayerModeAction; // Shift+S

    QAction *m_cycleDisplayLayerMode;
};

#endif // PCBEDITORWIDGET_H
