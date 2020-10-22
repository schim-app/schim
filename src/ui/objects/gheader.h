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
    GHeader(Header *obj);

    // GETTERS

    Header *get();
    const Header *get() const;

    // EVENTS

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;
};

#endif // GHEADER_H
