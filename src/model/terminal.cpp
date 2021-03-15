#include "terminal.h"

#include "model/compositeobject.h"
#include "model/component.h"

#include <QtMath>

Terminal::Terminal()
    : Object()
{
}

QList<Terminal::Prong> Terminal::getProngs() const
{
    QList<Prong> retVal;
    retVal.reserve(prongs.size());
    for (auto p : prongs)
        retVal.append(Prong(p, const_cast<Terminal*>(this)));
    return retVal;
}

QPointF Terminal::getSheetPos() const
{
    return getPos() + (getParent() ? getParent()->getPos() : QPointF{});
}

Component *Terminal::getParent() const
{
    return component;
}

void Terminal::setParent(Component *component)
{
    this->component = component;
}

void Terminal::addProng(float prong)
{
    prongs.append(prong);
}

// OBJECT INTERFACE

Object *Terminal::clone() const
{
    Terminal *obj = new Terminal;
    obj->setPos(getPos());
    obj->prongs = prongs; // Deep copy
    return new Terminal;
}

QPointF Terminal::getPos() const
{
    return pos;
}

void Terminal::setPos(const QPointF &pos)
{
    this->pos = pos;
}

Terminal::Prong::Prong(float angle, Terminal *terminal)
    : angle(angle), terminal(terminal)
{

}

float Terminal::Prong::getAngle() const
{
    return angle;
}

void Terminal::Prong::setAngle(float angle)
{
    this->angle = angle;
}

Terminal *Terminal::Prong::getTerminal() const
{
    return terminal;
}
