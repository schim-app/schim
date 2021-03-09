#include "terminal.h"

Terminal::Terminal()
    : Object()
{
}

QList<Terminal::Prong> &Terminal::getProngs()
{
    return prongs;
}

void Terminal::addProng(const Terminal::Prong &prong)
{
    prongs.append(prong);
}

int Terminal::convenience(const Terminal &terminal)
{
    return 0; // TODO impl
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

Terminal::Prong::Prong(float angle)
    : angle(angle)
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
