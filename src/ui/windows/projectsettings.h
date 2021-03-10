#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include "model/project.h"

#include <QDialog>

namespace Ui {
class ProjectSettings;
}

class ProjectSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectSettings(Project *project, QWidget *parent = nullptr);
    ~ProjectSettings();

private:
    Ui::ProjectSettings *ui;
    Project *project{};
};

#endif // PROJECTSETTINGS_H
