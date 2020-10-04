#include "sheetsettings.h"
#include "ui_sheetsettings.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

SheetSettings::SheetSettings(MainWindow *parent, int sheetId)
    : ui(new Ui::SheetSettings()), parent(parent), sheetId(sheetId)
{
    ui->setupUi(this);
}

SheetSettings::~SheetSettings()
{
    delete ui;
}

void SheetSettings::setSheetId(int id)
{
    sheetId = id;
}

void SheetSettings::show()
{
    parent->getCurrentSheet();
    ui->editSheetTitle->setText(parent->getCurrentSheet()->getTitle());
    ui->editSheetTitle->setFocus();
    QDialog::show();
}

void SheetSettings::accept()
{
    parent->getCurrentSheet()->setTitle(ui->editSheetTitle->text());
    parent->ui->tabView->setTabText(sheetId, ui->editSheetTitle->text());
    QDialog::accept();
}
