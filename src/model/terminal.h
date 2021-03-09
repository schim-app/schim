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
    QList<Prong> &getProngs();

    // SETTERS
    void addProng(const Prong &prong);

    /**
     * @brief How convenient would it be to connect this terminal to `terminal`.
     *
     * A physical measure we have introduced that is used to determine if a
     * connection should be suggested between this terminal and `terminal`.
     */
    int convenience(const Terminal &terminal);

    // OBJECT INTERFACE
    Object *clone() const;
    QPointF getPos() const;
    void setPos(const QPointF &pos);

private:
    // ATTRIBUTES
    QList<Prong> prongs;
    QPointF pos;
};

class Terminal::Prong
{
public:

    enum Type {
        Simple, Wide
    };

    Prong(float angle);

    float getAngle() const;
    void setAngle(float angle);

protected:
    float angle = 0;
};

class Terminal::WideProng
{
public:

private:
    float angle2 = 0;
};


#endif // TERMINAL_H
