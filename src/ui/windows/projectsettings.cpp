#include "projectsettings.h"
#include "ui_projectsettings.h"

#include "ui/mainwindow.h"
#include "global.h"

#include <QPushButton>

#include <iostream>

// Local helpers
QFont boldify(QFont font, bool bold)
{
    font.setBold(bold);
    return font;
}

ProjectSettings::ProjectSettings(QWidget *parent) :
    QDialog(parent), ui(new Ui::ProjectSettings)
{
    ui->setupUi(this);
    populateProjectComboBox();
    populateSelector();

    // Set initial category
    onCategorySelected(model->index(0, 0));
    // Load initial project
    loadProject(activeProjectIndex);

    // CONNECTIONS
    // When the user selects an item from the combo box
    connect(ui->comboProject,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ProjectSettings::loadProject);
    // When the user selects a settings category
    connect(ui->navigator->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &ProjectSettings::onCategorySelected);
    connect(ui->buttonBox->button(QDialogButtonBox::Apply),
            &QPushButton::clicked, this, &ProjectSettings::apply);
}

ProjectSettings::~ProjectSettings()
{
    delete ui;
    delete model;
}

void ProjectSettings::loadProject(int index)
{
    // Reset the bold-ness of the previous active project
    ui->comboProject->setItemData(activeProjectIndex,
                                  boldify(ui->comboProject->font(), false),
                                  Qt::FontRole);
    // Display the active project item in the popup list as BOLD
    ui->comboProject->setItemData(index,
                                  boldify(ui->comboProject->font(), true),
                                  Qt::FontRole);
    activeProjectIndex = index;

    // Load project data
    auto *proj = projects[index];
    ui->editName->setText(proj->getProperty("name"));
    ui->editAuthor->setText(proj->getProperty("author"));
    ui->editRevision->setText(proj->getProperty("revision"));
    ui->editStandard->setText(proj->getProperty("standard"));
}

void ProjectSettings::onCategorySelected(const QModelIndex &index)
{
    auto *item = model->itemFromIndex(index);
    auto id = index.row();
    // Change the category label on top of the page
    ui->labelCategory->setText(item->text());
    // Display the corresponding widget
    ui->stack->setCurrentIndex(id);
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

void ProjectSettings::populateSelector()
{
    model = new QStandardItemModel;

    // Create category items
    itemInfo = new QStandardItem("Project Info");
    itemVariables = new QStandardItem("Variables");
    itemDefaults = new QStandardItem("Defaults");

    // Make them read-only
    itemInfo->setEditable(false);
    itemVariables->setEditable(false);
    itemDefaults->setEditable(false);

    // Adjust their size
    itemInfo->setSizeHint({0, 40});
    itemVariables->setSizeHint({0, 40});
    itemDefaults->setSizeHint({0, 40});

    // Add them to the list model
    model->appendRow(itemInfo);
    model->appendRow(itemVariables);
    model->appendRow(itemDefaults);

    // Display
    ui->navigator->setModel(model);
    ui->navigator->setCurrentIndex(model->index(0, 0));
}

void ProjectSettings::populateProjectComboBox()
{
    // Add projects to the combo box
    auto *projModel = MainWindow::getInstance()->getProjectManager();
    projects = projModel->getProjects();
    for (int i = 0; i < projects.count(); ++i)
        ui->comboProject->addItem(projects[i]->getName(), i);

    // The initial project is the one that is active in the project browser
    activeProjectIndex = projects.indexOf(projModel->getActiveProject());
}
