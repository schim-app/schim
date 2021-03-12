#include "gterminal.h"

#include <iostream>
#include <QtMath>
#include "ui/objects/gcompositeobject.h"
#include <QApplication>
#include <QBrush>

GTerminal::GTerminal(Terminal *obj)
    : GObject(obj)
{
    setFlags({});
}

Terminal *GTerminal::get()
{
    return (Terminal*) obj;
}

const Terminal *GTerminal::get() const
{
    return (Terminal*) obj;
}

void GTerminal::reloadFromModel()
{
    setPos(get()->getPos());
}

void GTerminal::applyToModel()
{
    get()->setPos(pos());
}

QRectF GTerminal::boundingRect() const
{
    return {-2, -2, 4, 4};
}

void GTerminal::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *s, QWidget *w)
{
#define PRONG_LENGTH 2
    GObject::paint(painter, s, w);
    auto pen = painter->pen();
    painter->setBrush(painter->pen().color());
    painter->setPen(Qt::transparent);
    painter->drawEllipse({}, 0.6, 0.6);
    pen.setWidthF(0.25);
    painter->setPen(pen);
    if (!parentItem()->isSelected()) return;
    for (auto prong : get()->getProngs())
    {
        float angle = -qDegreesToRadians(prong.getAngle());
        float x = PRONG_LENGTH * qCos(angle);
        float y = PRONG_LENGTH * qSin(angle);
        painter->drawLine(0, 0, x, y);
    }
}

QVariant GTerminal::itemChange(QGraphicsItem::GraphicsItemChange change,
                               const QVariant &value)
{
    return GObject::itemChange(change, value);
}

GConnectionSuggester::GConnectionSuggester(
        const Terminal::Prong &p1,
        const Terminal::Prong &p2)
{
    // TODO use in a smarter way later
    setLine({p1.getTerminal()->getParent()->getPos() + p1.getTerminal()->getPos(),
            p2.getTerminal()->getParent()->getPos() + p2.getTerminal()->getPos()});
    setPen(QPen(qApp->palette().color(QPalette::Text), 0, Qt::DotLine));
}
