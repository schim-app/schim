#ifndef PROJECTBROWSER_H
#define PROJECTBROWSER_H

#include "model/project.h"

#include <QList>
#include <QTreeView>

class ProjectModel : public QAbstractItemModel
{
public:
    // CUSTOM ROLES
    static constexpr int EntityTypeRole = Qt::UserRole + 1;
    enum EntityType {
        ProjectEntity, SheetEntity
    };

    // CONSTRUCTORS
    explicit ProjectModel() = default;

    // GETTERS
    QList<Project*> &getProjects();
    Project* getActiveProject() const;
    QModelIndex getIndex(Entity *entity) const;

    // SETTERS
    void setActiveProject(Project *project);
    void addProject(Project *project);
    void addSheet(Sheet *sheet, int index = -1, Project *project = nullptr);
    void removeProject(Project *project);
    void deleteSheet(Sheet *sheet);

    // QAbstractItemModel INTERFACE
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
private: // These methods are not exposed to the interface
    int columnCount(const QModelIndex &parent) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent)
        const override;
    QModelIndex parent(const QModelIndex &child) const override;
    Qt::ItemFlags flags() const;

private:
    QList<Project*> projects;
    Project *activeProject{};
};

class ProjectBrowser : public QTreeView
{
    Q_OBJECT
public:
    explicit ProjectBrowser(QWidget *parent = nullptr);
    ~ProjectBrowser();

    ProjectModel *model();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QMenu *createProjectMenu(Project *project);
    QMenu *createSheetMenu(Sheet *sheet);
};

#endif // PROJECTBROWSER_H
