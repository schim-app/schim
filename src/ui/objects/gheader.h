#ifndef GHEADER_H
#define GHEADER_H

#include "gobject.h"
#include "model/header.h"

class GHeader : public GObject
{
public:
    GHeader(Header *obj);
};

class GDefaultHeader : public GHeader
{
};

#endif // GHEADER_H
