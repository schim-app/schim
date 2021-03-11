#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include "model/project.h"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui
{
class ProjectSettings;
}

class ProjectSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectSettings(QWidget *parent = nullptr);
    ~ProjectSettings();

private slots:
    void loadProject(int index);
    void onCategorySelected(const QModelIndex &index);
    // USER ACTIONS
    void apply();
    void accept() override;
    void reject() override;

private:
    // SETUP HELPERS
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
