#ifndef ENTITY_H
#define ENTITY_H

#include "variable.h"

class Entity
{
public:
    Entity() = default;
    virtual ~Entity() = default;

    // GETTERS
    QString getName() const;
    QString getFileName() const;

    // SETTERS
    void setName(const QString &name);
    void setFileName(const QString &name);

protected:
    QString name;
    QString filename;
};

#endif // ENTITY_H
