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

// SETTERS

void Project::addSheet(Sheet *sheet, int index)
{
    sheet->setProject(this);
    if (index == -1)
        sheets.append(sheet);
    else
        sheets.insert(index, sheet);
}

void Project::removeSheet(Sheet *sheet)
{
    sheet->setProject(nullptr);
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
