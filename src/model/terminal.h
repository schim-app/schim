#ifndef TERMINAL_H
#define TERMINAL_H

#include "object.h"

#include <QList>

/**
 |   |
/|\ /_\
**/

class Terminal : public Object
{
public:
    class Prong;
    class WideProng;

    Terminal();

    // GETTERS
    QList<Prong> getProngs() const;

    // SETTERS
    void addProng(float prong);

    // OBJECT INTERFACE
    Object *clone() const;
    QPointF getPos() const;
    void setPos(const QPointF &pos);

private:
    // ATTRIBUTES
    QList<float> prongs;
    QPointF pos;
};

class Terminal::Prong
{
public:

    enum Type {
        Simple, Wide
    };

    Prong(float angle, Terminal *terminal);

    float getAngle() const;
    void setAngle(float angle);
    Terminal *getTerminal() const;

protected:
    float angle = 0;
    Terminal *terminal{};
};

class Terminal::WideProng
{
public:

private:
    float angle2 = 0;
};


#endif // TERMINAL_H
