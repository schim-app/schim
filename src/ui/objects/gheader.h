#ifndef GHEADER_H
#define GHEADER_H

#include "gcompositeobject.h"
#include "model/header.h"

#include <QVariant>

/**
 * @brief Graphical representation of a `Header` in a scene.
 */
class GHeader : public GCompositeObject
{
public:
    // CONSTRUCTORS
    GHeader(Header *obj);

    // GETTERS
    virtual Header *get() override;
    virtual const Header *get() const override;
};

#endif // GHEADER_H
