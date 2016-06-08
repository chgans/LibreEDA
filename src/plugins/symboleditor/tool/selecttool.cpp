#include "tool/selecttool.h"
#include "tool/moveitemtool.h"
#include "tool/cloneitemtool.h"
#include "tool/dragselecttool.h"

#include "view/scene.h"
#include "view/view.h"
#include "item/item.h"
#include "handle/handle.h"

#include "propertyeditor/itempropertyeditor.h"
#include "objectinspector/objectinspectorview.h"
#include "objectinspector/objectinspectormodel.h"

#include "command/command.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>
#include <QHeaderView>
#include <QDebug>

using namespace SymbolEditor;

SelectTool::SelectTool(QObject *parent):
    InteractiveTool(parent)
{
    QAction *action = new QAction(QIcon::fromTheme("edit-select"),
                                  "Select <i>Esc</i>", nullptr);
    action->setShortcut(QKeySequence(Qt::Key_Escape));
    setAction(action);
    setToolGroup("interactive-tools");

    setupObjectInspector();
    setupPropertyBrowser();
    setTaskWidgets(QList<QWidget*>() << m_objectInspectorView << m_itemPropertyEditor);
}

SelectTool::~SelectTool()
{

}

void SelectTool::addDocumentItem(quint64 id, const Document::Item *item)
{
    m_objectInspectorModel->addTopLevelItem(id, item->friendlyTypeName(), item->icon());
    updateDocumentItem(id, item);
}

void SelectTool::updateDocumentItem(quint64 id, const Document::Item *item)
{
    m_objectInspectorModel->setItemVisibility(id, item->isVisible());
    m_objectInspectorModel->setItemLockState(id, item->isLocked());
}

void SelectTool::removeDocumentItem(quint64 id)
{
    m_objectInspectorModel->removeItem(id);
}

void SelectTool::onObjectInspectorVisibilityChangeRequested(quint64 id, bool visibility)
{
    qDebug() << "Set visibility" << id << visibility;
}

void SelectTool::onObjectInspectorLockStateChangeRequested(quint64 id, bool lockState)
{
    qDebug() << "Set Lock state" << id << lockState;
}

void SelectTool::onObjectInspectorSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (m_changingSelection)
    {
        return;
    }

    m_changingSelection = true;

    for (const QModelIndex &index: deselected.indexes())
    {
        auto id = m_objectInspectorModel->documentIdForIndex(index);
        auto item = scene()->itemForDocumentId(id);
        item->setSelected(false);
    }

    for (const QModelIndex &index: selected.indexes())
    {
        auto id = m_objectInspectorModel->documentIdForIndex(index);
        auto item = scene()->itemForDocumentId(id);
        item->setSelected(true);
    }

    m_changingSelection = false;
}

void SelectTool::onSceneSelectionChanged()
{
    m_itemPropertyEditor->setItem(nullptr); // or m_itemPropertyEditor->clear();
    for (auto item: scene()->selectedObjects())
    {
        // FIXME: only one item for now
        m_itemPropertyEditor->setItem(item);
        break;
    }

    if (m_changingSelection)
    {
        return;
    }

    m_changingSelection = true;

    auto selectionModel = m_objectInspectorView->selectionModel();
    selectionModel->clear();

    for (auto item: scene()->selectedObjects())
    {
        auto documentId = item->data(0).value<quint64>();
        auto modelIndex = m_objectInspectorModel->indexForDocumentId(documentId);
        selectionModel->select(modelIndex, QItemSelectionModel::Select|QItemSelectionModel::Rows);
    }

    m_changingSelection = false;

}

void SelectTool::cancel()
{
}

void SelectTool::activate(View *view)
{
    setView(view);
    onSceneSelectionChanged();
    connect(scene(), &Scene::selectionChanged,
            this, &SelectTool::onSceneSelectionChanged);
    connect(m_objectInspectorView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SelectTool::onObjectInspectorSelectionChanged);
    connect(m_objectInspectorModel, &ObjectInspectorModel::itemVisiblityChangeRequested,
            this, &SelectTool::onObjectInspectorVisibilityChangeRequested);
    connect(m_objectInspectorModel, &ObjectInspectorModel::itemLockStateChangeRequested,
            this, &SelectTool::onObjectInspectorLockStateChangeRequested);
    m_itemPropertyEditor->setItem(nullptr);
}

void SelectTool::desactivate()
{
    m_objectInspectorModel->disconnect(this);
    m_objectInspectorView->selectionModel()->disconnect(this);
    scene()->disconnect(this);
}

void SelectTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    event->accept();
}

void SelectTool::mouseMoveEvent(QMouseEvent *event)
{
    event->accept();
}

void SelectTool::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    auto item = view()->objectUnderMouse();
    if (item == nullptr)
    {
        scene()->clearSelection();
        event->accept();
        return;
    }

    if (!event->modifiers().testFlag(Qt::ShiftModifier))
    {
        scene()->clearSelection();
    }
    item->setSelected(!item->isSelected());
}

void SelectTool::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void SelectTool::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void SelectTool::setupObjectInspector()
{
    m_objectInspectorModel = new ObjectInspectorModel(this);
    m_objectInspectorView = new ObjectInspectorView;
    m_objectInspectorView->setModel(m_objectInspectorModel);
}

void SelectTool::setupPropertyBrowser()
{
    m_itemPropertyEditor = new ItemPropertyEditor;
}
