#ifndef GHEADER_H
#define GHEADER_H

#include "gobject.h"
#include "model/header.h"

#include <QVariant>

class GHeader : public GObject
{
public:
    GHeader(Header *obj);

    Header *get();
    const Header *get() const;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

};

#endif // GHEADER_H
