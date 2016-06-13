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

#include "command/setpropertycommand.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QAction>
#include <QHeaderView>
#include <QDebug>

namespace SymbolEditor
{

    SelectTool::SelectTool(Editor *editor):
        InteractiveTool(editor)
    {
        QAction *action = new QAction(QIcon::fromTheme("edit-select"),
                                      "Select <i>Esc</i>", nullptr);
        action->setShortcut(QKeySequence(Qt::Key_Escape));
        setAction(action);
        setToolGroup("interactive-tools");

        setupObjectInspector();
        setupPropertyBrowser();
        setTaskWidgets(QList<QWidget*>() << m_objectInspectorView << m_propertyEditor);
    }

    SelectTool::~SelectTool()
    {

    }

    void SelectTool::setPalette(Palette palette)
    {
        m_palette = palette;
        m_propertyEditor->setPalette(m_palette);
    }

    Palette SelectTool::palette() const
    {
        return m_palette;
    }

    void SelectTool::addDocumentItem(quint64 id, const Document::Item *item)
    {
        m_objectInspectorModel->addTopLevelItem(id, item->friendlyTypeName(), item->icon());
        m_objectInspectorModel->setItemVisibility(id, item->isVisible());
        m_objectInspectorModel->setItemLockState(id, item->isLocked());
    }

    void SelectTool::updateDocumentItem(quint64 id, const Document::Item *item)
    {
        m_objectInspectorModel->setItemVisibility(id, item->isVisible());
        m_objectInspectorModel->setItemLockState(id, item->isLocked());
    }

    void SelectTool::updateDocumentItemProperty(quint64 itemId, quint64 propertyId, const QVariant &value)
    {
        switch (propertyId)
        {
            case xdl::symbol::Item::VisibilityProperty:
                // FIXME: Force selected to false if item is being hidden?
                m_objectInspectorModel->setItemVisibility(itemId, value.toBool());
                break;
            case xdl::symbol::Item::LockedProperty:
                // FIXME: Force selected to false if item is being locked?
                m_objectInspectorModel->setItemLockState(itemId, value.toBool());
                break;
            default:
                break;
        }


        if (m_propertyEditor->item() != nullptr && m_propertyEditor->item()->id() == itemId)
        {
            m_propertyEditor->updateProperty(propertyId, value);
        }
    }

    void SelectTool::removeDocumentItem(quint64 id)
    {
        m_objectInspectorModel->removeItem(id);
    }

    void SelectTool::onObjectInspectorVisibilityChangeRequested(quint64 id, bool visibility)
    {
        auto command = new SetPropertyCommand();
        command->setItemId(id);
        command->setPropertId(xdl::symbol::Item::VisibilityProperty);
        command->setPropertyValue(visibility);
        emit commandRequested(command);
    }

    void SelectTool::onObjectInspectorLockStateChangeRequested(quint64 id, bool lockState)
    {
        auto command = new SetPropertyCommand();
        command->setItemId(id);
        command->setPropertId(xdl::symbol::Item::LockedProperty);
        command->setPropertyValue(lockState);
        emit commandRequested(command);
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
        m_propertyEditor->setItem(nullptr);
        for (auto item: scene()->selectedObjects())
        {
            quint64 id = scene()->documentIdForItem(item);
            m_propertyEditor->setItem(document()->item(id));
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

    void SelectTool::activate()
    {
        onSceneSelectionChanged();
        connect(scene(), &Scene::selectionChanged,
                this, &SelectTool::onSceneSelectionChanged);
        connect(m_objectInspectorView->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &SelectTool::onObjectInspectorSelectionChanged);
        connect(m_objectInspectorModel, &ObjectInspectorModel::itemVisiblityChangeRequested,
                this, &SelectTool::onObjectInspectorVisibilityChangeRequested);
        connect(m_objectInspectorModel, &ObjectInspectorModel::itemLockStateChangeRequested,
                this, &SelectTool::onObjectInspectorLockStateChangeRequested);
        connect(m_propertyEditor, &ItemPropertyEditor::commandRequested,
                this, &SelectTool::commandRequested);
    }

    void SelectTool::desactivate()
    {
        m_propertyEditor->disconnect(this);
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
        m_propertyEditor = new ItemPropertyEditor();
    }

}
