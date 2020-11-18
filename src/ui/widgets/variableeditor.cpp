#include "variableeditor.h"

#include "ui_variableeditor.h"

VariableEditor::VariableEditor(const Variable &variable, QWidget *parent)
    : QWidget(parent), ui(new Ui::VariableEditor), variable(variable)
{
    ui->setupUi(this);

    for (auto *child : children())
        child->installEventFilter(this);

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

void VariableEditor::setVariable(const Variable &var)
{
    variable = var;
    reload();
}

Variable VariableEditor::getVariable() const
{
    return variable;
}

bool VariableEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
        emit focused();
    return QWidget::eventFilter(obj, event);
}

void VariableEditor::updateVariableName()
{
    variable.name = ui->editName->text();
    if (!ui->editAbbreviations->text().isEmpty())
        for (auto str : ui->editAbbreviations->text().split(' '))
            variable.name += QString(" ") + str;
}

void VariableEditor::on_editName_textEdited(const QString &text)
{
    Q_UNUSED(text)
    // If the name has been typed in for the first time, create an abbreviation
    // that consists of the first character of the name
    if (text.size() == 1 && ui->editAbbreviations->text().isEmpty())
    {
        variable.name = text + " " + text[0];
        reload();
    }
    else
        updateVariableName();
    emit changed();
}

void VariableEditor::on_editAbbreviations_textEdited(const QString &text)
{
    Q_UNUSED(text)
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
