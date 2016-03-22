#ifndef _${Target}_H_${timestamp}_
#define _${Target}_H_${timestamp}_

#include <QGraphicsItem>

class ${Target} : public QGraphicsItem
{
public:
    ${Target}();
    explicit ${Target}(QGraphicsItem *parent = 0);

public:
    // returns an estimate of the area painted by the item
    QRectF boundingRect() const;

    // mplements the actual painting
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    // QPainterPath shape() const;

signals:

public slots:
};

#endif // _${Target}_H_${timestamp}_
