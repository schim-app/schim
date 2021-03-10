#include "projectsettings.h"
#include "ui_projectsettings.h"

ProjectSettings::ProjectSettings(Project *project, QWidget *parent) :
    QDialog(parent), ui(new Ui::ProjectSettings), project(project)
{
    ui->setupUi(this);
}

ProjectSettings::~ProjectSettings()
{
    delete ui;
}
