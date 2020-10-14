#ifndef GHEADER_H
#define GHEADER_H

#include "gcompositeobject.h"
#include "model/header.h"

#include <QVariant>

class GHeader : public GCompositeObject
{
public:
    GHeader(Header *obj);

    Header *get();
    const Header *get() const;

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;
};

#endif // GHEADER_H
