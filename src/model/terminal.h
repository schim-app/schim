#ifndef TERMINAL_H
#define TERMINAL_H

#include "model/object.h"
#include "model/entity.h"

#include <QList>

/**
 |   |
/|\ /_\
**/

class Component;

class Terminal : public Object
{
public:
    class Prong;
    class WideProng;

    Terminal();

    // GETTERS
    QList<Prong> getProngs() const;
    QPointF getSheetPos() const;
    Component *getParent() const;

    // SETTERS
    void setParent(Component *component);
    void addProng(float prong);

    // OBJECT INTERFACE
    virtual Object *clone() const override;
    virtual QPointF getPos() const override;
    virtual void setPos(const QPointF &pos) override;

private:
    // ATTRIBUTES
    QList<float> prongs;
    QPointF pos;
    Component *component{};
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
