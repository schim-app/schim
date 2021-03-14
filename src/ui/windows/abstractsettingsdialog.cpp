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
    ui->navigator->installEventFilter(this);
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

void AbstractSettingsDialog::mousePressEvent(QMouseEvent *event)
{
    QDialog::mousePressEvent(event);
    // On click, unfocus any edit boxes
    if (dynamic_cast<QLineEdit*>(focusWidget()))
    {
        focusWidget()->clearFocus();
        ui->navigator->setFocus();
    }
}

bool AbstractSettingsDialog::eventFilter(QObject *obj, QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
    {
        auto *event = (QKeyEvent*) e;
        if (obj == ui->navigator)
        {
            // TODO bug: the key isn't registered on first press. Only when you
            // press it the second time this method gets called
            Vim::registerKeyPress(event, [this](const Vim::Action &action) {
                return processVimAction(action);
            });
        }
    }
    return QDialog::eventFilter(obj, e);
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

bool AbstractSettingsDialog::processVimAction(const Vim::Action &action)
{
    auto i = ui->navigator->currentIndex().row();
    if (action == "down")
    {
        int next = i + 1 >= model->rowCount() ? model->rowCount() - 1 : i + 1;
        ui->navigator->setCurrentIndex(model->index(next, 0));
    }
    else if (action == "up")
    {
        int prev = i != 0 ? i - 1 : 0;
        ui->navigator->setCurrentIndex(model->index(prev, 0));
    }
    else return false;
    return true;
}
