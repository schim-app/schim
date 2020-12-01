#include "sheet.h"

#include "line.h"
#include "global.h"
#include "fileio/xml.h"

Sheet::Sheet(bool defaultHeader)
{
    if (defaultHeader)
        header = xmlParseHeader(QString(resolvePath("headers/defaultheader.xsym")));
}

Sheet::~Sheet()
{
    qDeleteAll(*this);
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

QRectF Sheet::getContentArea() const
{
    if (header)
        return header->getContentArea();
    else
        return Header().getContentArea();
}

VariableSet &Sheet::getLocalVariables()
{
    return variables;
}

VariableSet Sheet::getLocalVariables() const
{
    return variables;
}

VariableSet Sheet::getVariables() const
{
    VariableSet vars = variables;
    if (project)
        vars.append(project->getVariables());

    return vars;
}

void Sheet::setWidth(float width)
{
    this->width = width;
}

void Sheet::setHeight(float height)
{
    this->height = height;
}

void Sheet::setTitle(const QString &title)
{
    this->title = title;
}

void Sheet::setHeader(Header *header)
{
    delete this->header;
    this->header = header;
}
