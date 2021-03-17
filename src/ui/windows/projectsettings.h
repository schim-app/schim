/// @file projectsettings.h
#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include "ui/windows/abstractsettingsdialog.h"

#include "model/project.h"

#include <QComboBox>

namespace Ui { class ProjectSettings; }

/**
 * @brief Dialog to change the project settings.
 */
class ProjectSettings : public AbstractSettingsDialog
{
    Q_OBJECT

public:
    // CONSTRUCTORS
    explicit ProjectSettings(QWidget *parent = nullptr);
    ~ProjectSettings();

signals:
    void nameChanged(int projectId);

protected slots:
    // USER ACTIONS
    void apply() override;
    void accept() override;
    void reject() override;

private:
    // HELPERS
    void initializeProjectComboBox();
    /**
     * @brief Load a project's data into all the widgets.
     */
    void loadProject(int index);

    // ATTRIBUTES
    QList<Project*> projects;
    Project *project{};
    int activeProjectIndex;
    QComboBox *comboProject;
    Ui::ProjectSettings *ui{};
};

#endif // PROJECTSETTINGS_H
