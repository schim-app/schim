#include "project.h"

Project::Project()
    : QList()
{
}

Project::Project(const QList<Sheet *> &sheets)
    : QList(sheets)
{
}

Project::~Project()
{
    qDeleteAll(*this);
}

QString Project::getTitle() const
{
    return title;
}

VariableSet &Project::getVariables()
{
    return variables;
}

VariableSet Project::getVariables() const
{
    return variables;
}

void Project::setTitle(const QString &title)
{
    this->title = title;
}
