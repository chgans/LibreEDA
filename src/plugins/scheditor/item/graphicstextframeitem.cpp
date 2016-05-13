#include "item/graphicstextframeitem.h"

#include <QGraphicsTextItem>
#include <QStyleOptionGraphicsItem>
#include <QTextDocument>
#include <QTextCursor>

/* FIXME:
 *  - The clipping doesn't do what we want, we need to clip with some margin
 *  - The clipping clip the handle as wells
 * => the m_maskItem seems to do the trick and seems to be efficient
 * FIXME:
 *  - Add more handles like for GraphicsRectItem?
 *  - Use an existing Item for drawing the frame? => sticky not, rounded rect,
 *    ellipse, ... or allow to attach text to any shape item?
 *  - Fix font size, need mm/px scaling?
 *  - general polishing
 *  - use handle.setEnabled(true|false), make handles draw themselves differently
 *    when enabled/disabled.
 */

GraphicsTextFrameItem::GraphicsTextFrameItem(SchItem *parent):
    SchItem(parent),
    m_maskItem(new QGraphicsRectItem(this)),
    m_textItem(new QGraphicsTextItem(m_maskItem))
{
    addRegularHandle(0, MoveHandleRole, DiamondedHandleShape);
    m_sizePolicy = ResizeToContentPolicy;
    handleAt(0)->setEnabled(false);

    // Mask item is slightly smalller than the frame and will clip the text item
    m_maskItem->setPos(0, 0);
    m_maskItem->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    //m_maskItem->setFlag(QGraphicsItem::ItemHasNoContents);
    m_maskItem->setPen(Qt::NoPen);
    m_maskItem->setBrush(Qt::NoBrush);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    m_textItem->setPos(0, 0);
    m_textItem->document()->setPageSize(QSizeF(100, 100));
    QTextOption textOption = m_textItem->document()->defaultTextOption();
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    m_textItem->document()->setDefaultTextOption(textOption);
    m_textItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    connect(m_textItem, &QGraphicsTextItem::linkActivated,
            this, &GraphicsTextFrameItem::linkActivated);
    connect(m_textItem, &QGraphicsTextItem::linkHovered,
            this, &GraphicsTextFrameItem::linkHovered);
    connect(document(), &QTextDocument::contentsChanged,
            this, &GraphicsTextFrameItem::onDocumentContentChanged);
}

void GraphicsTextFrameItem::onDocumentContentChanged()
{
    if (m_sizePolicy == FixedSizePolicy) {
        return;
    }
    adjustSize();
}

void GraphicsTextFrameItem::adjustSize()
{
    prepareGeometryChange();
    m_textItem->adjustSize();
    m_frameRect = m_textItem->boundingRect();
    m_maskItem->setRect(m_textItem->boundingRect());
    m_boundingRect = QRectF();
    blockItemNotification();
    handleAt(0)->setPos(m_frameRect.bottomRight());
    unblockItemNotification();
    update();
}

GraphicsTextFrameItem::SizePolicy GraphicsTextFrameItem::sizePolicy() const
{
    return m_sizePolicy;
}

void GraphicsTextFrameItem::setSizePolicy(GraphicsTextFrameItem::SizePolicy policy)
{
    if (m_sizePolicy == policy)
        return;
    m_sizePolicy = policy;
    if (m_sizePolicy == ResizeToContentPolicy)
        adjustSize();
    handleAt(0)->setEnabled(m_sizePolicy == FixedSizePolicy);
}

QString GraphicsTextFrameItem::toHtml() const
{
    return m_textItem->toHtml();
}

// FIXME: Need same resizing behaviour as setPlainText
void GraphicsTextFrameItem::setHtml(const QString &html)
{
    m_textItem->setHtml(html);
}

QString GraphicsTextFrameItem::toPlainText() const
{
    return m_textItem->toPlainText();
}

void GraphicsTextFrameItem::setPlainText(const QString &text)
{
    m_textItem->setPlainText(text);
    m_textItem->adjustSize();
    if (m_sizePolicy == FixedSizePolicy)
        handleAt(0)->setPos(m_textItem->boundingRect().bottomRight());
    else
        adjustSize();
}

QFont GraphicsTextFrameItem::font() const
{
    return m_textItem->font();
}

void GraphicsTextFrameItem::setFont(const QFont &font)
{
    m_textItem->setFont(font);
}

void GraphicsTextFrameItem::setDefaultTextColor(const QColor &color)
{
    m_textItem->setDefaultTextColor(color);
}

QColor GraphicsTextFrameItem::defaultTextColor() const
{
    return m_textItem->defaultTextColor();
}

void GraphicsTextFrameItem::setDocument(QTextDocument *document)
{
    if (m_textItem->document() != nullptr)
        disconnect(m_textItem->document());
    m_textItem->setDocument(document);
    if (document != nullptr)
        connect(document, &QTextDocument::contentsChanged,
                this, &GraphicsTextFrameItem::onDocumentContentChanged);
}

QTextDocument *GraphicsTextFrameItem::document() const
{
    return m_textItem->document();
}

void GraphicsTextFrameItem::setTextInteractionFlags(Qt::TextInteractionFlags flags)
{
    m_textItem->setTextInteractionFlags(flags);
}

Qt::TextInteractionFlags GraphicsTextFrameItem::textInteractionFlags() const
{
    return m_textItem->textInteractionFlags();
}

bool GraphicsTextFrameItem::tabChangesFocus() const
{
    return m_textItem->tabChangesFocus();
}

void GraphicsTextFrameItem::setOpenExternalLinks(bool open)
{
    m_textItem->setOpenExternalLinks(open);
}

bool GraphicsTextFrameItem::openExternalLinks() const
{
    return m_textItem->openExternalLinks();
}

void GraphicsTextFrameItem::setTextCursor(const QTextCursor &cursor)
{
    m_textItem->setTextCursor(cursor);
}

QTextCursor GraphicsTextFrameItem::textCursor() const
{
    return m_textItem->textCursor();
}

QRectF GraphicsTextFrameItem::boundingRect() const
{
    if (m_boundingRect.isNull()) {
        qreal halfpw = pen().style() == Qt::NoPen ? qreal(0) : pen().widthF() / 2;
        m_boundingRect = m_sizePolicy == FixedSizePolicy ? m_frameRect : m_textItem->boundingRect();
        if (halfpw > 0.0)
            m_boundingRect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    }
    return m_boundingRect;
}

QPainterPath GraphicsTextFrameItem::shape() const
{
    QRectF rect = m_sizePolicy == FixedSizePolicy ? m_frameRect : m_textItem->boundingRect();
    QPainterPath path;
    qreal hpw = pen().widthF()/2.0;
    path.addRect(rect.adjusted(hpw, hpw, -hpw, -hpw));
    return path; //shapeFromPath(path, pen());
}

bool GraphicsTextFrameItem::contains(const QPointF &point) const
{
    return boundingRect().contains(point);
}

void GraphicsTextFrameItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(m_frameRect);
}

void GraphicsTextFrameItem::itemNotification(IGraphicsObservableItem *item)
{
    AbstractGraphicsHandle *handle = dynamic_cast<AbstractGraphicsHandle*>(item);
    Q_ASSERT(handle && handle->handleId() == 0);

    if (m_sizePolicy == ResizeToContentPolicy)
        return;
    prepareGeometryChange();
    m_frameRect.setBottomRight(handle->pos());
    m_textItem->setTextWidth(m_frameRect.width());
    m_maskItem->setRect(m_frameRect.adjusted(5, 5, -5, -5));
    m_textItem->update(); // Force invalidating cache?
    m_boundingRect = QRectF();
    update();
}

SchItem *GraphicsTextFrameItem::clone()
{
    GraphicsTextFrameItem *item = new GraphicsTextFrameItem();
    SchItem::cloneTo(item);
    item->setFont(font());
    item->setPlainText(toPlainText());
    item->setSizePolicy(sizePolicy());
    item->setDefaultTextColor(defaultTextColor());
    item->setTextInteractionFlags(textInteractionFlags());
    //item->setTabChangesFocus(tabChangesFocus());

    return item;
}

QVariant GraphicsTextFrameItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        for (AbstractGraphicsHandle *handle: m_handleToId.keys()) {
            handle->setVisible(isSelected());
        }
    }
    return value;
}
