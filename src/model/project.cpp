#include "project.h"

// CONSTRUCTORS

Project::Project()
{
    setName("Untitled project");
}

Project::Project(const QList<Sheet *> &sheets)
    : Project()
{
}

Project::~Project()
{
    qDeleteAll(getSheets());
}

QString Project::getProperty(const QString name)
{
    if (properties.contains(name))
        return properties[name];
    else if (name == "name")
        return getName();
    else
        std::logic_error("Project has no such property: " + name.toStdString());
    return {};
}

// GETTERS

VariableSet &Project::getVariables()
{
    return variables;
}

VariableSet Project::getVariables() const
{
    return variables;
}

QList<Sheet *> &Project::getSheets()
{
    return sheets;
}

QList<Sheet *> Project::getSheets() const
{
    return sheets;
}

void Project::setProperty(const QString &name, const QString &value)
{
    if (properties.contains(name))
        properties[name] = value;
    else if (name == "name")
        setName(value);
    else
        std::logic_error("Project has no such property: " + name.toStdString());
}

// SETTERS

void Project::addSheet(Sheet *sheet, int index)
{
    sheet->setParent(this);
    if (index == -1)
        sheets.append(sheet);
    else
        sheets.insert(index, sheet);
}

void Project::removeSheet(Sheet *sheet)
{
    sheet->setParent(nullptr);
    sheets.removeOne(sheet);
}

// MISCELLANEOUS

QList<Sheet *>::iterator Project::begin()
{
    return sheets.begin();
}

QList<Sheet *>::iterator Project::end()
{
    return sheets.end();
}
