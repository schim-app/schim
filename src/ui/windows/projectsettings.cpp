#include "projectsettings.h"
#include "ui_projectsettings.h" // TODO rm?

#include "ui/mainwindow.h"
#include "global.h"

#include <QPushButton>

#include <iostream>

// CONSTRUCTORS

ProjectSettings::ProjectSettings(QWidget *parent) :
    AbstractSettingsDialog(parent), ui(new Ui::ProjectSettings)
{
    addCategory("Project Info");
    addCategory("Variables");
    addCategory("Defaults");

    ui->setupUi(getStackedWidget());

    // Initialize AbstractSettingsDialog
    initialize();

    initializeProjectComboBox();

    // Load initial project
    loadProject(activeProjectIndex);
}

ProjectSettings::~ProjectSettings()
{
    delete ui;
}

// USER ACTIONS

void ProjectSettings::apply()
{
    auto *proj = projects[activeProjectIndex];
    proj->setProperty("name", ui->editName->text());
    proj->setProperty("author", ui->editAuthor->text());
    proj->setProperty("revision", ui->editRevision->text());
    proj->setProperty("standard", ui->editStandard->text());
}

void ProjectSettings::accept()
{
    apply();
    QDialog::accept();
}

void ProjectSettings::reject()
{
    // TODO show warning if changes were made
    QDialog::reject();
}

// HELPERS

void ProjectSettings::initializeProjectComboBox()
{
    // Add a combo box for choosing the configured project
    comboProject = new QComboBox(this);
    auto *title = getTitleLayout();
    title->addSpacerItem(new QSpacerItem(20, 1, QSizePolicy::Expanding));
    title->addWidget(new QLabel("Project:", this));
    title->addWidget(comboProject);

    // Add projects to the combo box
    auto *pm = MainWindow::getInstance()->getProjectManager();
    projects = pm->getProjects();
    for (int i = 0; i < projects.count(); ++i)
        comboProject->addItem(projects[i]->getName(), i);

    // The initial project is the one that is active in the project browser
    activeProjectIndex = projects.indexOf(pm->getActiveProject());

    // Callback when the user selects an item from the combo box
    connect(comboProject,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ProjectSettings::loadProject);
}

// Local helpers
QFont boldify(QFont font, bool bold)
{
    font.setBold(bold);
    return font;
}

void ProjectSettings::loadProject(int index)
{
    // Display the new active project item in the popup list as BOLD
    comboProject->setItemData(index,
                                  boldify(comboProject->font(), true),
                                  Qt::FontRole);
    // Reset the bold-ness of the previous active project
    comboProject->setItemData(activeProjectIndex,
                                  boldify(comboProject->font(), false),
                                  Qt::FontRole);
    activeProjectIndex = index;

    // Load project data
    auto *proj = projects[index];
    ui->editName->setText(proj->getProperty("name"));
    ui->editAuthor->setText(proj->getProperty("author"));
    ui->editRevision->setText(proj->getProperty("revision"));
    ui->editStandard->setText(proj->getProperty("standard"));
}
