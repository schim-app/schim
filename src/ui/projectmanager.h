/// @file projectmanager.h
#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "model/project.h"

#include <QList>
#include <QModelIndex>

/**
 * @brief A model that manages open projects.
 *
 * The model is a tree with a depth of 2. The top level contains a list of open
 * projects and the lower level contains all the sheets that exist in a given
 * project. One project can be set as the active project. Only sheets in the
 * active project can be opened for editing.
 */
class ProjectManager : public QAbstractItemModel
{
public:
    // CUSTOM ROLES
    static constexpr int EntityTypeRole = Qt::UserRole + 1;
    enum EntityType {
        ProjectEntity, SheetEntity
    };

    // CONSTRUCTORS
    explicit ProjectManager() = default;

    // GETTERS
    QList<Project*> &getProjects();
    Project* getActiveProject() const;
    /**
     * @brief Get the model index of the project or sheet that `entity` points to.
     */
    QModelIndex getIndex(Entity *entity) const;
    /**
     * @brief Return true if no projects are open, false otherwise.
     */
    bool isEmpty() const;

    // SETTERS
    /**
     * @brief Set the active project.
     * This will notify all views that the new active project and the previous
     * active project have changed.
     */
    void setActiveProject(Project *project);
    /**
     * @brief Add a project to the manager.
     *
     * The project manager takes ownership of `project`. Any views will be
     * notified that a model row was inserted.
     *
     * @note If the source file name of `project` matches the file name of
     * another open project, nothing will be done.
     */
    void addProject(Project *project);
    /**
     * @brief Add a new sheet to `project` and register it in the manager.
     *
     * The project takes ownership of the sheet. Any views will be notified that
     * a model row was inserted.
     *
     * @param index The index of `sheet` in the `project`.
     * @param project The proj
     */
    void addSheet(Sheet *sheet, int index = -1, Project *project = nullptr);
    /**
     * @brief Remove `project` from the manager and destroy it.
     *
     * Any views will be notified that a model row was removed.
     * If `project` was the active project, it will be deactivated.
     */
    void removeProject(Project *project);
    /**
     * @brief Remove `sheet` from the manager and from its project.
     *
     * Any views will be notified that a model row was removed.
     */
    void removeSheet(Sheet *sheet);

    // OVERRIDE QAbstractItemModel
    /**
     * @brief Get the model data.
     */
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
private: // These methods are not exposed to the interface
    int columnCount(const QModelIndex &parent) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent)
        const override;
    QModelIndex parent(const QModelIndex &child) const override;
    Qt::ItemFlags flags() const;

private:
    /// List of all open projects
    QList<Project*> projects;
    /// The active project
    Project *activeProject{};
};

#endif // PROJECTMANAGER_H
