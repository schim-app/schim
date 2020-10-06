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
    for (auto *sheet : *this)
        delete sheet;
}

QString Project::getTitle() const
{
    return title;
}

void Project::setTitle(const QString &title)
{
    this->title = title;
}
