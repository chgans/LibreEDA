#ifndef GRAPHICSTEXTFRAMEITEM_H
#define GRAPHICSTEXTFRAMEITEM_H

#include "schitem.h"

#include <QRectF>

class QGraphicsTextItem;

// TODO: Auto resize to content
// TODO: Wrap mode: QTextOption::WrapMode of item's document
class GraphicsTextFrameItem: public SchItem
{
    Q_OBJECT

public:
    enum SizePolicy {
        ResizeToContentPolicy = 0,
        FixedSizePolicy
    };

    GraphicsTextFrameItem(SchItem *parent = 0);


private:
    mutable QRectF m_boundingRect;
    QRectF m_frameRect;
    QGraphicsRectItem *m_maskItem;
    QGraphicsTextItem *m_textItem;
    SizePolicy m_sizePolicy;

private slots:
    void onDocumentContentChanged();

    // Facade pattern to QGraphicsTextItem
public:
    SizePolicy sizePolicy() const;
    void setSizePolicy(SizePolicy policy);

    void setTextWidth(qreal width); // TBD
    qreal textWidth() const; // TBD
    void adjustSize();

    QString toHtml() const;
    void setHtml(const QString &html);
    QString toPlainText() const;
    void setPlainText(const QString &text);
    QFont font() const;
    void setFont(const QFont &font);
    void setDefaultTextColor(const QColor &color);
    QColor defaultTextColor() const;
    // TBD: defaultFont, defaultTextBackgroundColor
    void setDocument(QTextDocument *document);
    QTextDocument *document() const;
    void setTextInteractionFlags(Qt::TextInteractionFlags flags);
    Qt::TextInteractionFlags textInteractionFlags() const;
    void setTabChangesFocus(bool b);
    bool tabChangesFocus() const;
    void setOpenExternalLinks(bool open);
    bool openExternalLinks() const;
    void setTextCursor(const QTextCursor &cursor);
    QTextCursor textCursor() const;
signals:
    void linkActivated(const QString &);
    void linkHovered(const QString &);


    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual bool contains(const QPointF &point) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    // IGraphicsItemObserver interface
public:
    virtual void itemNotification(IGraphicsObservableItem *item);

    // SchItem interface
public:
    virtual SchItem *clone();
    virtual bool fromJson(QString *errorString, const QJsonObject &jsonObject);
    virtual void toJson(QJsonObject &jsonObject) const;

    // QGraphicsItem interface
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // GRAPHICSTEXTFRAMEITEM_H
