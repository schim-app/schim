#ifndef ELEMENT_H
#define ELEMENT_H

#include <QPointF>

class Object
{
private:
    QPointF pos;

protected:

public:
    Object();
    virtual ~Object();

    QPointF getPos() const;

    void setPos(const QPointF &pos);

    friend class GObject;

};

#endif // ELEMENT_H
