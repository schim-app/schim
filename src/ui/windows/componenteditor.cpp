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
    {
        auto *varEditor = new VariableEditor(var, this);
        variableEditors.append(varEditor);
        ui->layout->addWidget(varEditor);
        connect(varEditor, &VariableEditor::changed, this, &ComponentEditor::onChanged);
    }
}

ComponentEditor::~ComponentEditor()
{
    delete ui;
}

void ComponentEditor::reload()
{
    // TODO
}

void ComponentEditor::onChanged()
{
    changed = true;
}

void ComponentEditor::accept()
{
    component->getVariables().clear();
    for (auto editor : variableEditors)
        component->addVariable(editor->getVariable());

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
