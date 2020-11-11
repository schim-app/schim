#include "variableeditor.h"

#include "ui_variableeditor.h"

VariableEditor::VariableEditor(const Variable &variable, QWidget *parent)
    : QWidget(parent), ui(new Ui::VariableEditor), variable(variable)
{
    ui->setupUi(this);
    reload();
}

VariableEditor::~VariableEditor()
{
    delete ui;
}

void VariableEditor::reload()
{
    auto nameAbbrevList = variable.name.split(" ");
    if (!nameAbbrevList.empty())
        ui->editName->setText(nameAbbrevList[0]);
    else
        ui->editName->clear();

    ui->editAbbreviations->setText(QString(variable.name).replace(QRegExp("^[^ ]{1,} "), ""));
    ui->editValue->setText(variable.value);
    ui->editDescription->setText(variable.description);
}

Variable VariableEditor::getVariable() const
{
    return variable;
}

void VariableEditor::updateVariableName()
{
    variable.name = ui->editName->text();
    for (auto str : ui->editAbbreviations->text().split(' '))
        variable.name += QString(" ") + str;
}

void VariableEditor::on_editName_textEdited(const QString &text)
{
    updateVariableName();
    emit changed();
}

void VariableEditor::on_editAbbreviations_textEdited(const QString &text)
{
    updateVariableName();
    emit changed();
}

void VariableEditor::on_editValue_textEdited(const QString &text)
{
    variable.value = text;
    emit changed();
}

void VariableEditor::on_editDescription_textEdited(const QString &text)
{
    variable.description = text;
    emit changed();
}
