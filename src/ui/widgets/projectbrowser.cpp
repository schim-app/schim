#include "projectbrowser.h"

#include "global.h"
#include "model/sheet.h"
#include "ui/mainwindow.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <iostream>

// class ProjectBrowser *********************************************************

ProjectBrowser::ProjectBrowser(QWidget *parent)
    : QTreeView(parent)
{
}

ProjectBrowser::~ProjectBrowser()
{
    delete model();
}

ProjectModel *ProjectBrowser::model()
{
    return (ProjectModel*) QTreeView::model();
}

void ProjectBrowser::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        auto index = indexAt(event->pos());
        if (index.data(ProjectModel::EntityTypeRole).toInt()
                == ProjectModel::ProjectEntity)
        {
            MainWindow::getInstance()->setActiveProject(
                        (Project*) index.internalPointer());
        }
        else
        {
            Sheet *sheet = (Sheet*) index.internalPointer();
            if (model()->getActiveProject() == sheet->getParent())
            { // Sheet belongs to the active project and is not already open
                int id = MainWindow::getInstance()->openSheet(sheet);
                MainWindow::getInstance()->getTabWidget()->setCurrentIndex(id);
            }
        }
    }
    else
        QTreeView::mouseDoubleClickEvent(event);
}

void ProjectBrowser::contextMenuEvent(QContextMenuEvent *event)
{
    // Handle right click
    auto index = indexAt(event->pos());
    if (!index.isValid()) return;
    // Item type - project or sheet
    int type = index.data(ProjectModel::EntityTypeRole).toInt();

    // Create a context menu depending on the type of the selected item
    QMenu *menu;
    if (type == ProjectModel::ProjectEntity)
        menu = createProjectMenu((Project*) index.internalPointer());
    else
        menu = createSheetMenu((Sheet*) index.internalPointer());

    menu->exec(event->globalPos());
    delete menu;
}

QMenu *ProjectBrowser::createProjectMenu(Project *project)
{
    QMenu *menu = new QMenu(this);
    QAction *actionSetActive = menu->addAction("Set as active project"),
            *actionClose = menu->addAction("Close project"),
            *actionNewSheet = menu->addAction("New sheet"),
            *actionOpenAll = menu->addAction("Open all sheets"),
            *actionCloseAll = menu->addAction("Close all sheets");

    if (project == model()->getActiveProject())
        actionSetActive->setDisabled(true);
    else
        actionOpenAll->setDisabled(true);
    // Connections
    connect(actionSetActive, &QAction::triggered, [project]() {
        MainWindow::getInstance()->setActiveProject(project);
    });
    connect(actionClose, &QAction::triggered, [project]() {
        MainWindow::getInstance()->closeProject(project);
    });
    connect(actionNewSheet, &QAction::triggered, [this, project]() {
        int count = project->getSheets().count();
        model()->addSheet(new Sheet, count, project);
    });
    connect(actionOpenAll, &QAction::triggered, [project]() {
        for (auto *sheet : project->getSheets())
            MainWindow::getInstance()->openSheet(sheet);
    });
    connect(actionCloseAll, &QAction::triggered, [project]() {
        for (auto *sheet : project->getSheets())
            MainWindow::getInstance()->closeSheet(sheet);
    });
    return menu;
}

QMenu *ProjectBrowser::createSheetMenu(Sheet *sheet)
{
    QMenu *menu = new QMenu(this);
    auto *actionOpenSheet = menu->addAction("Open sheet"),
            *actionDelete = menu->addAction("Delete sheet"),
            *actionSheetBefore = menu->addAction("New sheet before"),
            *actionSheetAfter = menu->addAction("New sheet after"),
            *actionSheetStart = menu->addAction("New sheet at start"),
            *actionSheetEnd = menu->addAction("New sheet at end");

    if (MainWindow::getInstance()->isOpen(sheet)
            || model()->getActiveProject() != sheet->getParent())
        actionOpenSheet->setDisabled(true);
    connect(actionDelete, &QAction::triggered, [this, sheet]() {
        MainWindow::getInstance()->closeSheet(sheet);
        model()->deleteSheet(sheet);
    });
    connect(actionOpenSheet, &QAction::triggered, [sheet]() {
        MainWindow::getInstance()->openSheet(sheet);
    });
    connect(actionSheetBefore, &QAction::triggered, this, [this, sheet]() {
        model()->addSheet(new Sheet, sheet->getIndex(), sheet->getParent());
    });
    connect(actionSheetAfter, &QAction::triggered, this, [this, sheet]() {
        model()->addSheet(new Sheet, sheet->getIndex() + 1, sheet->getParent());
    });
    connect(actionSheetStart, &QAction::triggered, this, [this, sheet]() {
        model()->addSheet(new Sheet, 0, sheet->getParent());
    });
    connect(actionSheetEnd, &QAction::triggered, this, [this, sheet]() {
        int count = sheet->getParent()->getSheets().count();
        model()->addSheet(new Sheet, count, sheet->getParent());
    });
    return menu;
}

// class ProjectModel ***********************************************************

// GETTERS

QList<Project *> &ProjectModel::getProjects()
{
    return projects;
}

Project *ProjectModel::getActiveProject() const
{
    return activeProject;
}

QModelIndex ProjectModel::getIndex(Entity *entity) const
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

bool ProjectModel::isEmpty() const
{
    return rowCount({}) == 0;
}

// SETTERS

void ProjectModel::setActiveProject(Project *project)
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

void ProjectModel::addProject(Project *project)
{
    for (const auto *proj : projects)
        if (proj->getFileName() != "" && resolveAbsPath(proj->getFileName())
                == resolveAbsPath(project->getFileName()))
            return;
    beginInsertRows({}, projects.size(), projects.size());
    projects.append(project);
    endInsertRows();
}

void ProjectModel::addSheet(Sheet *sheet, int index, Project *project)
{
    if (project == nullptr) project = getActiveProject();

    beginInsertRows(getIndex(project), index, index);
    project->addSheet(sheet, index);
    endInsertRows();
}

void ProjectModel::removeProject(Project *project)
{
    auto row = projects.indexOf(project);
    beginRemoveRows({}, row, row);
    if (projects.isEmpty())
        setActiveProject(nullptr);
    projects.removeAt(row);
    endRemoveRows();
}

void ProjectModel::deleteSheet(Sheet *sheet)
{
    Project *project = sheet->getParent();
    int index = sheet->getIndex();
    beginRemoveRows(getIndex(project), index, index);
    project->removeSheet(sheet);
    delete sheet;
    endRemoveRows();
}

// QAbstractItemModel INTERFACE

QVariant ProjectModel::data(const QModelIndex &index, int role) const
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

int ProjectModel::rowCount(const QModelIndex &parent) const
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

int ProjectModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex ProjectModel::parent(const QModelIndex &child) const
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

Qt::ItemFlags ProjectModel::flags() const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
