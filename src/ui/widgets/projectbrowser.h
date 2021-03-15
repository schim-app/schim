/// @file projectbrowser.h
#ifndef PROJECTBROWSER_H
#define PROJECTBROWSER_H

#include "ui/projectmanager.h"
#include "model/project.h"

#include <QTreeView>

/**
 * @brief The project browser that is usually found in a docked widget in the
 * main window.
 *
 * A tree view that uses a `ProjectManager` as its underlying model.
 */
class ProjectBrowser : public QTreeView
{
    Q_OBJECT
public:
    explicit ProjectBrowser(QWidget *parent = nullptr);

    ProjectManager *model();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    // HELPERS
    QMenu *createProjectMenu(Project *project);
    QMenu *createSheetMenu(Sheet *sheet);
};

#endif // PROJECTBROWSER_H
