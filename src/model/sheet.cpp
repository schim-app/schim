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
    qDeleteAll(objects);
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

QList<Object *> &Sheet::getObjects()
{
    return objects;
}

QList<Object *> Sheet::getObjects() const
{
    return objects;
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

void Sheet::setProject(Project *project)
{
    this->project = project;
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

void Sheet::setLocalVariables(const VariableSet &vars)
{
    variables.clear();
    for (auto var : vars)
        if (var.getTrueName() != "")
            variables.append(var);
}

void Sheet::addVariable(const Variable &variable)
{
    for (const auto &var : variables)
        if (var == variable)
            return;
    variables.append(variable);
}

void Sheet::addObject(Object *obj)
{
    objects.append(obj);
    obj->setSheet(this);
}

void Sheet::removeObject(Object *obj)
{
    // TODO check if this sheet owns the object?
    objects.removeOne(obj);
    obj->setSheet(nullptr);
}

QList<Object *>::iterator Sheet::begin()
{
    return objects.begin();
}

QList<Object *>::iterator Sheet::end()
{
    return objects.end();
}
