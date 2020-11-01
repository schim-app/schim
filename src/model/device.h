#ifndef DEVICE_H
#define DEVICE_H

#include <QList>
#include <QMap>

class Component;

class Device
{
public:
    Device();

private:
    QList<Component*> components{};
};

#endif // DEVICE_H
