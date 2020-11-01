#include "componenteditor.h"

#include "ui_componenteditor.h"

ComponentEditor::ComponentEditor(QWidget *parent)
    : QDialog(parent), ui(new Ui::ComponentEditor)
{
    ui->setupUi(this);
}

ComponentEditor::~ComponentEditor()
{
    delete ui;
}
