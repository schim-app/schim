#include "projectmanager.h"

#include "global.h"

#include <QFont>

// GETTERS

QList<Project *> &ProjectManager::getProjects()
{
    return projects;
}

Project *ProjectManager::getActiveProject() const
{
    return activeProject;
}

QModelIndex ProjectManager::getIndex(Entity *entity) const
{
    if (dynamic_cast<Project*>(entity))
        return index(projects.indexOf((Project*) entity), 0, {});
    else if (dynamic_cast<Sheet*>(entity))
    {
        Sheet *sheet = static_cast<Sheet*>(entity);
        Project *proj = sheet->getParent();
        return index(sheet->getIndex(), 0, getIndex(proj));
    }
    else
        return {};
}

bool ProjectManager::isEmpty() const
{
    return rowCount({}) == 0;
}

// SETTERS

void ProjectManager::setActiveProject(Project *project)
{
    Project *previousActive = activeProject;
    activeProject = project;
    auto index = getIndex(project);
    if (project != nullptr)
        // New active project's name becomes bold
        emit dataChanged(index, index, {Qt::FontRole});
    if (previousActive != nullptr)
    { // Previous active project loses bold name
        index = getIndex(previousActive);
        emit dataChanged(index, index, {Qt::FontRole});
    }
}

void ProjectManager::addProject(Project *project)
{
    for (auto *proj : projects)
        if (proj->getFileName() != "" && resolveAbsPath(proj->getFileName())
                == resolveAbsPath(project->getFileName()))
            return;
    beginInsertRows({}, projects.size(), projects.size());
    projects.append(project);
    endInsertRows();
}

void ProjectManager::addSheet(Sheet *sheet, int index, Project *project)
{
    if (project == nullptr) project = getActiveProject();

    beginInsertRows(getIndex(project), index, index);
    project->addSheet(sheet, index);
    endInsertRows();
}

void ProjectManager::removeProject(Project *project)
{
    auto row = projects.indexOf(project);
    beginRemoveRows({}, row, row);
    if (projects.isEmpty())
        setActiveProject(nullptr);
    projects.removeAt(row);
    delete project;
    endRemoveRows();
}

void ProjectManager::removeSheet(Sheet *sheet)
{
    Project *project = sheet->getParent();
    int index = sheet->getIndex();
    beginRemoveRows(getIndex(project), index, index);
    project->removeSheet(sheet);
    delete sheet;
    endRemoveRows();
}

// OVERRIDE QAbstractItemModel

QVariant ProjectManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};
    Entity *ptr = static_cast<Entity*>(index.internalPointer());
    if (role == Qt::DisplayRole)
        return ptr->getName() + (ptr == getActiveProject() ? " [active]" : "");
    else if (role == Qt::ToolTipRole)
        return dynamic_cast<Project*>(ptr) ?
                    resolveAbsPath(ptr->getFileName()) : "";
    else if (role == EntityTypeRole)
        return dynamic_cast<Sheet*>((Entity*) index.internalPointer()) ?
                    SheetEntity : ProjectEntity;
    else if (role == Qt::FontRole)
    {
        QFont font;
        font.setBold(ptr == getActiveProject());
        return font;
    }
    else
        return {};
}

int ProjectManager::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) return projects.size();
    Entity *ptr = static_cast<Entity*>(parent.internalPointer());
    if (dynamic_cast<Project*>(ptr))
        return static_cast<Project*>(ptr)->getSheets().size();
    else if (dynamic_cast<Sheet*>(ptr))
        return 0;
    else
        return 0;
}

int ProjectManager::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex ProjectManager::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};
    if (!parent.isValid()) // Parent is root item
        return createIndex(row, column, projects[row]);
    Entity *ptr = static_cast<Entity*>(parent.internalPointer());
    if (dynamic_cast<Project*>(ptr)) // Parent is a project and not a sheet
        return createIndex(row, column,
                           static_cast<Project*>(ptr)->getSheets()[row]);
    else
        return {};

}

QModelIndex ProjectManager::parent(const QModelIndex &child) const
{
    if (!child.isValid()) return {};
    Entity *ptr = static_cast<Entity*>(child.internalPointer());
    if (dynamic_cast<Project*>(ptr))
        return {};
    else
    {
        auto *sheet = static_cast<Sheet*>(ptr);
        return createIndex(projects.indexOf(sheet->getParent()),
                           0, sheet->getParent());
    }
}

Qt::ItemFlags ProjectManager::flags() const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
