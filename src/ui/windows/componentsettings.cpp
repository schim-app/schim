#include "componentsettings.h"

#include "ui_componentsettings.h"

#include <QMessageBox>
#include <QScrollBar>

ComponentSettings::ComponentSettings(GComponent *component, QWidget *parent)
    : QDialog(parent), ui(new Ui::ComponentEditor), component(component)
{
    ui->setupUi(this);

    // Initialization

    ui->variableEditor->setVariables(component->get()->getVariables());
    reload();
    // TODO Why do I have to connect this here when I have clicked "Go to slot" in QtDesigner?
    connect(ui->editFunction, &QLineEdit::textEdited, this, &ComponentSettings::on_editFunction_textEdited);
    connect(ui->editLocation, &QLineEdit::textEdited, this, &ComponentSettings::on_editLocation_textEdited);
    connect(ui->editDesignation, &QLineEdit::textEdited, this, &ComponentSettings::on_editDesignation_textEdited);

    connect(ui->variableEditor, &VariableEditor::changed, this, &ComponentSettings::onChanged);
}

ComponentSettings::~ComponentSettings()
{
    delete ui;
}

// UPDATE METHODS

void ComponentSettings::reload()
{
    auto variables = component->get()->getVariables();
    ui->variableEditor->reload();
    updateDesignatorFields();
}

QString ComponentSettings::compileDesignator()
{
    QString retVal;
    if (ui->editFunction->text() != "")
        retVal += "=" + ui->editFunction->text();
    if (ui->editLocation->text() != "")
        retVal += "+" + ui->editLocation->text();
    if (ui->editDesignation->text() != "")
        retVal += "-" + ui->editDesignation->text();
    return retVal;
}

void ComponentSettings::updateDesignatorVariable()
{
    /* TODO rewrite
    VariableSet &vars = component->get()->getLocalVariables();
    for (auto *editor : variableEditors)
    {
        Variable var = editor->getVariables();
        if (var.name.contains("designation"))
        {
            var.value = compileDesignator();
            editor->setVariable(var);
            return;
        }
    }
    vars.insert(0, {"designation d dt", compileDesignator()});
    // There is no variable named designator
    addVariableEditor(vars.first(), 0);
    */
}

void ComponentSettings::updateDesignatorFields()
{
    Variable var = Variable::find(ui->variableEditor->getVariables(), "designation");
    QRegExp equal("=[^=+-]+"); // To parse the function
    QRegExp plus("\\+[^=+-]+"); // To parse the location
    QRegExp dash("-[^=+-]+"); // To parse the designation
    if (equal.indexIn(var.value) != -1)
    ui->editFunction->setText(equal.capturedTexts().at(0).mid(1));
    if (plus.indexIn(var.value) != -1)
    ui->editLocation->setText(plus.capturedTexts().at(0).mid(1));
    if (dash.indexIn(var.value) != -1)
    ui->editDesignation->setText(dash.capturedTexts().at(0).mid(1));
    if (QRegExp("[^=\\+-]+").exactMatch(var.value))
    ui->editDesignation->setText(var.value);
}

// CUSTOM SLOTS

void ComponentSettings::onChanged()
{
    changed = true;
    // In case the designator variable has changed
    updateDesignatorFields();
}

void ComponentSettings::onChildFocused()
{
    // TODO apparently unused -- remove
    //focusedEditor = (VariableEditor *) sender();
}

void ComponentSettings::accept()
{
    // TODO move this to VariableEditor?
    VariableSet &vars = component->get()->getLocalVariables();
    vars.clear();
    for (auto var : ui->variableEditor->getVariables())
        if (var.getTrueName() != "")
            vars.append(var);

    QDialog::accept();
    component->reload();
}

void ComponentSettings::reject()
{
    if (!changed ||
        QMessageBox::question(this, "Are you sure?","Are you sure you want to discard changes to this component?")
            == QMessageBox::Yes
       )
    {
        QDialog::reject();
    }
}

// UI SLOTS

void ComponentSettings::on_editFunction_textEdited(const QString &text)
{
    Q_UNUSED(text)
    updateDesignatorVariable();
    changed = true;
}

void ComponentSettings::on_editLocation_textEdited(const QString &text)
{
    Q_UNUSED(text)
    updateDesignatorVariable();
    changed = true;
}

void ComponentSettings::on_editDesignation_textEdited(const QString &text)
{
    Q_UNUSED(text)
    updateDesignatorVariable();
    changed = true;
}
