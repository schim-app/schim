#include "abstractsettingsdialog.h"
#include "ui_abstractsettingsdialog.h"

#include "ui/mainwindow.h"
#include "global.h"

#include <QPushButton>

#include <iostream>

AbstractSettingsDialog::AbstractSettingsDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::AbstractSettingsDialog)
{
    ui->setupUi(this);
    // List model for the navigator
    model = new QStandardItemModel;
}

AbstractSettingsDialog::~AbstractSettingsDialog()
{
    delete ui;
    delete model;
}

// INITIALIZATION

void AbstractSettingsDialog::addCategory(const QString &name)
{
    auto *item = new QStandardItem(name);
    item->setEditable(false);
    item->setSizeHint({0, 40});
    model->appendRow(item);
}

void AbstractSettingsDialog::initialize()
{
    ui->navigator->setModel(model);
    ui->navigator->setCurrentIndex(model->index(0, 0));
    onCategorySelected(model->index(0, 0));

    // When the user selects a setting category
    connect(ui->navigator->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &AbstractSettingsDialog::onCategorySelected);
    // When the user presses a dialog button
    connect(ui->buttonBox->button(QDialogButtonBox::Apply),
            &QPushButton::clicked, this, &AbstractSettingsDialog::apply);
}

// GETTERS

QHBoxLayout *AbstractSettingsDialog::getTitleLayout()
{
    return ui->titleLayout;
}

QStackedWidget *AbstractSettingsDialog::getStackedWidget()
{
    return ui->stackedWidget;
}

// USER ACTIONS

void AbstractSettingsDialog::apply()
{
}

void AbstractSettingsDialog::accept()
{
    apply();
    QDialog::accept();
}

void AbstractSettingsDialog::reject()
{
    // TODO show warning if changes were made
    QDialog::reject();
}

// PRIVATE SLOTS
void AbstractSettingsDialog::onCategorySelected(const QModelIndex &index)
{
    auto *item = model->itemFromIndex(index);
    auto id = index.row();
    // Change the category label on top of the page
    ui->labelCategory->setText(item->text());
    // Display the corresponding widget
    ui->stackedWidget->setCurrentIndex(id);
}
