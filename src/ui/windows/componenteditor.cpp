#include "componenteditor.h"

#include "ui_componenteditor.h"

#include <QMessageBox>

ComponentEditor::ComponentEditor(Component *component, QWidget *parent)
    : QDialog(parent), ui(new Ui::ComponentEditor), component(component)
{
    ui->setupUi(this);

    // Initialization
    auto variables = component->getVariables();
    for (auto var : variables)
        addVariableEditor(var);

    // TODO Why do I have to connect this here when I have clicked "Go to slot" in QtDesigner?
    connect(ui->btnAddVariable, &QAbstractButton::clicked, this, &ComponentEditor::on_btnAddVariable_clicked);
    connect(ui->btnRemoveVariable, &QAbstractButton::clicked, this, &ComponentEditor::on_btnRemoveVariable_clicked);
}

ComponentEditor::~ComponentEditor()
{
    delete ui;
}

void ComponentEditor::reload()
{
    // TODO
}

void ComponentEditor::addVariableEditor(const Variable &var)
{
    auto *varEditor = new VariableEditor(var);
    variableEditors.append(varEditor);
    ui->variablesView->layout()->addWidget(varEditor);

    connect(varEditor, &VariableEditor::changed, this, &ComponentEditor::onChanged);
    connect(varEditor, &VariableEditor::focused, this, &ComponentEditor::onChildFocused);
}

void ComponentEditor::onChanged()
{
    changed = true;
}

void ComponentEditor::onChildFocused()
{
    focusedEditor = (VariableEditor *) sender();
}

void ComponentEditor::accept()
{
    VariableSet &vars = component->getLocalVariables();;
    vars.clear();
    for (auto editor : variableEditors)
        if (editor->getVariable().name != "" && Variable::find(vars, editor->getVariable().name).name == "")
            vars.append(editor->getVariable());

    QDialog::accept();
}

void ComponentEditor::reject()
{
    if (!changed ||
        QMessageBox::question(this, "Are you sure?","Are you sure you want to discard changes to this component?")
            == QMessageBox::Yes
       )
    {
        QDialog::reject();
    }
}

void ComponentEditor::on_btnAddVariable_clicked()
{
    addVariableEditor({});
    changed = true;
}

void ComponentEditor::on_btnRemoveVariable_clicked()
{
    if (focusedEditor)
    {
        variableEditors.removeOne(focusedEditor);
        ui->variablesView->layout()->removeWidget(focusedEditor);
        delete focusedEditor;
    }

    focusedEditor = nullptr;
    changed = true;
}
