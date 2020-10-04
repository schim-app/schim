#ifndef ELEMENT_H
#define ELEMENT_H

#include <QList>

class Object
{

    QList<Object*> children;

public:
    Object();
    ~Object();

    virtual void addChild(Object *child);
};

#endif // ELEMENT_H
