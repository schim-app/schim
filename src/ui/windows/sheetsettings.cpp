#include "sheetsettings.h"
#include "ui_sheetsettings.h"

#include "ui/mainwindow.h"

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
