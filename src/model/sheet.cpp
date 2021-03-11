#include "sheet.h"

#include "line.h"
#include "global.h"
#include "fileio/xml.h"

// CONSTRUCTORS

Sheet::Sheet(bool defaultHeader)
{
    setName("New sheet");
    if (defaultHeader)
    {
        QString file = "headers/defaultheader.xsym";
        header = xmlParseHeader(QString(resolvePath(file)));
        header->setSourceFile(file);
    }
}

Sheet::~Sheet()
{
    qDeleteAll(objects);
    delete header;
}

// GETTERS

float Sheet::getWidth() const
{
    return width;
}

float Sheet::getHeight() const
{
    return height;
}

Header *Sheet::getHeader()
{
    return header;
}

QRectF Sheet::getContentArea() const
{
    if (header)
        return header->getContentArea();
    else
        return Header().getContentArea();
}

QList<Object*> &Sheet::getObjects()
{
    return objects;
}

QList<Object*> Sheet::getObjects() const
{
    return objects;
}

int Sheet::getIndex()
{
    return getParent() ? getParent()->getSheets().indexOf(this) : -1;
}

Project *Sheet::getParent() const
{
    return dynamic_cast<Project*>(Entity::getParent());
}

// SETTERS

void Sheet::setWidth(float width)
{
    this->width = width;
}

void Sheet::setHeight(float height)
{
    this->height = height;
}

void Sheet::setHeader(Header *header, bool destroy)
{
    if (header == this->header)
        return;

    if (destroy)
        delete this->header;
    this->header = header;
    if (header != nullptr)
        header->setParent(this);
}

void Sheet::addObject(Object *obj)
{
    objects.append(obj);
    // TODO preliminary remove obj->setParent(this);
}

void Sheet::removeObject(Object *obj)
{
    // TODO check if this sheet owns the object?
    objects.removeOne(obj);
    // TODO preliminary remove obj->setParent(nullptr);
}

void Sheet::setParent(Project *project)
{
    Entity::setParent(project);
}

// MISCELLANEOUS

QList<Object*>::iterator Sheet::begin()
{
    return objects.begin();
}

QList<Object*>::iterator Sheet::end()
{
    return objects.end();
}
