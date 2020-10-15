#include "sheet.h"

//TODO temporary
#include "line.h"
#include <dxflib/dl_dxf.h>
#include <fileio/dxf.h>

Sheet::Sheet()
{
}

Sheet::~Sheet()
{
    for (auto *obj : *this)
        delete obj;
    delete header;
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

void Sheet::setTitle(const QString &title)
{
    this->title = title;
}
