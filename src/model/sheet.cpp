#include "sheet.h"

//TODO temporary
#include "line.h"
#include <dxflib/dl_dxf.h>
#include <fileio/dxf.h>

Sheet::Sheet()
{
    //TODO temporary
    DL_Dxf *dxf = new DL_Dxf();
    auto *obj = new CompositeObject;
    DXFFilter filter(obj);
    bool success = dxf->in("../staging/no-contact.dxf", &filter);
    delete dxf;
    delete obj;
    //ODOT temporary

}

Sheet::~Sheet()
{
    for (auto *obj : objects)
        delete obj;
    delete header;
}

void Sheet::addObject(Object *obj)
{
    objects.append(obj);
}

float Sheet::getWidth() const
{
    return width;
}

float Sheet::getHeight() const
{
    return height;
}

QString Sheet::getTitle() const
{
    return title;
}

Header *Sheet::getHeader()
{
    return header;
}

QList<Object *> &Sheet::getObjects()
{
    return objects;
}

void Sheet::setTitle(const QString &title)
{
    this->title = title;
}
