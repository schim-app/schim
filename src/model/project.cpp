#include "project.h"

// CONSTRUCTORS

Project::Project(const QList<Sheet *> &sheets)
{
}

Project::~Project()
{
    qDeleteAll(getSheets());
}

// GETTERS

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

QList<Sheet *> &Project::getSheets()
{
    return sheets;
}

QList<Sheet *> Project::getSheets() const
{
    return sheets;
}

// SETTERS

void Project::setTitle(const QString &title)
{
    this->title = title;
}

void Project::addSheet(Sheet *sheet)
{
    sheet->setProject(this);
    sheets.append(sheet);
}

void Project::removeSheet(Sheet *sheet)
{
    sheet->setProject(nullptr);
    sheets.append(sheet);
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
