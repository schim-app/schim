/// @file projectsettings.h
#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include "model/project.h"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui { class ProjectSettings; }

/**
 * @brief Dialog to change the project settings.
 */
class ProjectSettings : public QDialog
{
    Q_OBJECT

public:
    // CONSTRUCTORS
    explicit ProjectSettings(QWidget *parent = nullptr);
    ~ProjectSettings();

private slots:
    /**
     * @brief Load a project's data into all the widgets.
     */
    void loadProject(int index);
    /**
     * @brief Called when a user changes a settings category.
     */
    void onCategorySelected(const QModelIndex &index);

    // USER ACTIONS
    void apply();
    void accept() override;
    void reject() override;

private:
    // HELPERS
    void populateSelector();
    void populateProjectComboBox();

    // ATTRIBUTES
    Ui::ProjectSettings *ui;
    QList<Project*> projects;
    Project *project{};
    QStandardItem *itemInfo{}, *itemVariables{}, *itemDefaults{};
    QStandardItemModel *model{};
    int activeProjectIndex;

};

#endif // PROJECTSETTINGS_H
