#include "sheetsettings.h"
#include "ui_sheetsettings.h"

#include "ui/mainwindow.h"
#include "fileio/xml.h"
#include "global.h"

#include <QMessageBox>

// TODO rm
#include <QDebug>

SheetSettings::SheetSettings(MainWindow *parent, int sheetId)
    : ui(new Ui::SheetSettings), parent(parent), sheetId(sheetId)
{
    ui->setupUi(this);
    connect(ui->variableEditor, &VariableEditor::changed, this, &SheetSettings::onChanged);

    // Initialize widgets with sheet data
    auto *sheet = parent->getSheet();


    ui->editSheetTitle->setText(sheet->getTitle());
    ui->editSheetTitle->setFocus();
    if (sheet->getHeader() != nullptr)
    {
        originalHeader = sheet->getHeader()->getSourceFile();
        ui->editHeader->setText(sheet->getHeader()->getSourceFile());
    }
    else
        originalHeader = "";

    ui->variableEditor->setVariables(parent->getSheet()->getLocalVariables());

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

void SheetSettings::onChanged()
{
    _changed = true;
}

void SheetSettings::accept()
{
    // TODO handle invalid file names properly
    if (ui->editHeader->text() != "")
    {
        Header *hdr = xmlParseHeader(resolvePath(ui->editHeader->text()));
        hdr->setSourceFile(ui->editHeader->text());
        if (!scene()->tryChangeHeader(hdr))
        {
            delete hdr;
            return;
        }
    }
    else
        scene()->setHeader(nullptr);

    parent->getSheet()->setTitle(ui->editSheetTitle->text());
    parent->getTabView()->setTabText(sheetId, ui->editSheetTitle->text());
    parent->getSheet()->setLocalVariables(ui->variableEditor->getVariables());

    scene()->reload();
    QDialog::accept();
}

void SheetSettings::reject()
{
    if (!_changed ||
        QMessageBox::question(this, "Are you sure?", "Discard changes?")
            == QMessageBox::Yes)
    {
        QDialog::reject();
    }
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

void SheetSettings::on_btnHeader_triggered(QAction *action)
{
}

void SheetSettings::on_btnClearHeader_clicked()
{
    ui->editHeader->clear();
    _changed = true;
}

void SheetSettings::on_editHeader_textEdited(const QString &text)
{
    _changed = true;
}
