#include "sheetsettings.h"
#include "ui_sheetsettings.h"

#include "ui/mainwindow.h"

SheetSettings::SheetSettings(MainWindow *parent, int sheetId)
    : ui(new Ui::SheetSettings), parent(parent), sheetId(sheetId)
{
    ui->setupUi(this);

    ui->cbShowGrid->setChecked(scene()->isGridEnabled());
    ui->cbSnapGrid->setChecked(scene()->isSnapEnabled());
    ui->sbGridX->setValue(scene()->getGridSize().width());
    ui->sbGridY->setValue(scene()->getGridSize().height());
}

SheetSettings::~SheetSettings()
{
    delete ui;
}

void SheetSettings::setSheetId(int id)
{
    sheetId = id;
}

SheetScene *SheetSettings::scene() const
{
    return parent->getTab()->scene();
}

void SheetSettings::show()
{
    parent->getSheet();
    ui->editSheetTitle->setText(parent->getSheet()->getTitle());
    ui->editSheetTitle->setFocus();
    QDialog::show();
}

void SheetSettings::accept()
{
    parent->getSheet()->setTitle(ui->editSheetTitle->text());
    parent->getTabView()->setTabText(sheetId, ui->editSheetTitle->text());
    QDialog::accept();
}

// SLOTS

void SheetSettings::on_cbShowGrid_toggled(bool checked)
{
    scene()->setGridEnabled(checked);
    scene()->update();
}

void SheetSettings::on_cbSnapGrid_toggled(bool checked)
{
    scene()->setSnapToGrid(checked);
    scene()->update();
}

void SheetSettings::on_sbGridX_valueChanged(double value)
{
    if (ui->cbGridConstrain->isChecked())
        ui->sbGridY->setValue(value);

    scene()->setGridSize(value, ui->sbGridY->value());
    scene()->update();
}

void SheetSettings::on_sbGridY_valueChanged(double value)
{
    if (ui->cbGridConstrain->isChecked())
        ui->sbGridX->setValue(value);

    scene()->setGridSize(ui->sbGridX->value(), value);
    scene()->update();
}
