#include "scheditor.h"
#include "scheditorwidget.h"
#include "scheditordocument.h"
#include "schview.h"
#include "schscene.h"
#include "item/schitem.h"
#include "scheditorsettings.h"

#include "core/core.h"

#include <QFileInfo>

SchEditor::SchEditor(QObject *parent) :
    IEditor(parent)
{
    setWidget(new SchEditorWidget());
}

SchEditor::~SchEditor()
{
    delete m_document;
}

void SchEditor::loadSettings()
{
    SchEditorSettings settings;
    settings.load(Core::settings());

    schWidget()->view()->setPaletteMode(settings.colorScheme);
    schWidget()->view()->setRulerEnabled(settings.rulerEnabled);
    schWidget()->view()->setGridEnabled(settings.gridEnabled);
    schWidget()->view()->setMinimalGridSize(int(settings.minimalGridSize)); // FIXME: int vs uint
    schWidget()->view()->setGridCoarseMultiplier(int(settings.coarseGridMultiplier));
    if (settings.solidCoarseGridLinesEnabled)
    {
        schWidget()->view()->setGridCoarseLineStyle(Qt::SolidLine);
    }
    else
    {
        schWidget()->view()->setGridCoarseLineStyle(Qt::DotLine);
    }
    if (settings.solidFineGridLinesEnabled)
    {
        schWidget()->view()->setGridFineLineStyle(Qt::SolidLine);
    }
    else
    {
        schWidget()->view()->setGridFineLineStyle(Qt::DotLine);
    }

    if (!settings.scrollBarsEnabled)
    {
        schWidget()->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        schWidget()->view()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else if (settings.scrollBarsAsNeededEnabled)
    {
        schWidget()->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        schWidget()->view()->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    else
    {
        schWidget()->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        schWidget()->view()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
    if (!settings.cursorCrosshairEnabled)
    {
        schWidget()->view()->setMouseCursor(SchView::NoMouseCursor);
    }
    else if (settings.largeCursorCrosshairEnabled)
    {
        schWidget()->view()->setMouseCursor(SchView::LargeMouseCursor);
    }
    else
    {
        schWidget()->view()->setMouseCursor(SchView::SmallMouseCursor);
    }
    if (!settings.originCrosshairEnabled)
    {
        schWidget()->view()->setOriginMark(SchView::NoOriginMark);
    }
    else if (settings.largeOriginCrosshairEnabled)
    {
        schWidget()->view()->setOriginMark(SchView::LargeOriginMark);
    }
    else
    {
        schWidget()->view()->setOriginMark(SchView::SmallOriginMark);
    }
    schWidget()->view()->setRenderHint(QPainter::Antialiasing, settings.antiAliasingEnabled);
    schWidget()->view()->setRenderHint(QPainter::TextAntialiasing, settings.antiAliasingEnabled);
    schWidget()->view()->setRenderHint(QPainter::HighQualityAntialiasing, settings.antiAliasingEnabled);
    schWidget()->view()->setHardwareAccelerationEnabled(settings.hardwareAccelerationEnabled);
}


bool SchEditor::open(QString *errorString, const QString &fileName)
{
    m_document = new SchEditorDocument();
    m_document->setFilePath(fileName);
    QFileInfo fileInfo(fileName);
    m_document->setDisplayName(fileInfo.baseName());
    bool result = m_document->load(errorString, m_document->filePath());
    if (!result)
        return false;
    foreach (SchItem *item, m_document->items()) {
        schWidget()->scene()->addItem(item);
    }
    return true;
}

IDocument *SchEditor::document() const
{
    return m_document;
}

SchEditorWidget *SchEditor::schWidget() const
{
    return static_cast<SchEditorWidget *>(widget());
}

QIcon SchEditor::icon() const
{
    return QIcon(":/icons/sch.png");
}

QString SchEditor::displayName() const
{
    return m_document->displayName();
}


void SchEditor::activate(QMainWindow *mainWindow)
{
    schWidget()->activate(mainWindow);
}

void SchEditor::desactivate(QMainWindow *mainWindow)
{
    schWidget()->desactivate(mainWindow);
}
