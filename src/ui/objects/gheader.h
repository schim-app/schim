/// @file gheader.h
#ifndef GHEADER_H
#define GHEADER_H

#include "gcompositeobject.h"
#include "model/header.h"

#include <QVariant>

/**
 * @brief Graphical object that wraps a `Header`.
 */
class GHeader : public GCompositeObject
{
public:
    // CONSTRUCTORS
    explicit GHeader(Header *obj);

    // GETTERS
    Header *get() override;
    const Header *get() const override;
};

#endif // GHEADER_H
