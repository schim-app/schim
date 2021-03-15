#include "projectbrowser.h"

#include "global.h"
#include "model/sheet.h"
#include "ui/mainwindow.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <iostream>

ProjectBrowser::ProjectBrowser(QWidget *parent)
    : QTreeView(parent)
{
}

ProjectManager *ProjectBrowser::model()
{
    return (ProjectManager*) QTreeView::model();
}

void ProjectBrowser::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        auto index = indexAt(event->pos());
        if (!index.isValid()) return; // No item was clicked
        if (index.data(ProjectManager::EntityTypeRole).toInt()
                == ProjectManager::ProjectEntity)
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
    int type = index.data(ProjectManager::EntityTypeRole).toInt();

    // Create a context menu depending on the type of the selected item
    QMenu *menu;
    if (type == ProjectManager::ProjectEntity)
        menu = createProjectMenu((Project*) index.internalPointer());
    else
        menu = createSheetMenu((Sheet*) index.internalPointer());

    menu->exec(event->globalPos());
    delete menu;
}

// HELPERS

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
        model()->removeSheet(sheet);
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
