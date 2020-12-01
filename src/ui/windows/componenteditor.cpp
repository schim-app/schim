#include "componenteditor.h"

#include "ui_componenteditor.h"

#include <QMessageBox>
#include <QScrollBar>

ComponentEditor::ComponentEditor(GComponent *component, QWidget *parent)
    : QDialog(parent), ui(new Ui::ComponentEditor), component(component)
{
    ui->setupUi(this);

    // Initialization

    reload();
    // TODO Why do I have to connect this here when I have clicked "Go to slot" in QtDesigner?
    connect(ui->btnAddVariable, &QAbstractButton::clicked, this, &ComponentEditor::on_btnAddVariable_clicked);
    connect(ui->btnRemoveVariable, &QAbstractButton::clicked, this, &ComponentEditor::on_btnRemoveVariable_clicked);
    connect(ui->editFunction, &QLineEdit::textEdited, this, &ComponentEditor::on_editFunction_textEdited);
    connect(ui->editLocation, &QLineEdit::textEdited, this, &ComponentEditor::on_editLocation_textEdited);
    connect(ui->editDesignation, &QLineEdit::textEdited, this, &ComponentEditor::on_editDesignation_textEdited);
}

ComponentEditor::~ComponentEditor()
{
    delete ui;
}

// UPDATE METHODS

void ComponentEditor::reload()
{
    auto variables = component->get()->getVariables();
    foreach (auto var, variables)
        addVariableEditor(var);

    updateDesignatorFields();
}

void ComponentEditor::addVariableEditor(const Variable &var, int index)
{
    auto *varEditor = new VariableEditor(var);
    variableEditors.append(varEditor);
    auto *layout = dynamic_cast<QVBoxLayout*>(ui->variablesView->layout());
    if (index >= 0)
        layout->insertWidget(index, varEditor);
    else
        layout->addWidget(varEditor);

    connect(varEditor, &VariableEditor::changed, this, &ComponentEditor::onChanged);
    connect(varEditor, &VariableEditor::focused, this, &ComponentEditor::onChildFocused);
}

QString ComponentEditor::compileDesignator()
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

void ComponentEditor::updateDesignatorVariable()
{
    VariableSet &vars = component->get()->getLocalVariables();
    for (auto *editor : variableEditors)
    {
        Variable var = editor->getVariable();
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
}

void ComponentEditor::updateDesignatorFields()
{
    ui->editFunction->clear();
    ui->editLocation->clear();
    ui->editDesignation->clear();
    for (auto *editor : variableEditors)
    {
        Variable var = editor->getVariable();
        if (var.name.contains("designation"))
        {
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
            return;
        }
    }
}

void ComponentEditor::timerEvent(QTimerEvent *event)
{
    QDialog::timerEvent(event);
    auto *scrollBar = ui->scrollArea->verticalScrollBar();
    scrollBar->setSliderPosition(scrollBar->maximum());
}

// CUSTOM SLOTS

void ComponentEditor::onChanged()
{
    changed = true;
    // In case the designator variable has changed
    updateDesignatorFields();
}

void ComponentEditor::onChildFocused()
{
    focusedEditor = (VariableEditor *) sender();
}

// UI SLOTS

void ComponentEditor::accept()
{
    VariableSet &vars = component->get()->getLocalVariables();;
    vars.clear();
    for (auto *editor : variableEditors)
        if (editor->getVariable().name != "" && Variable::find(vars, editor->getVariable().name).name == "")
            vars.append(editor->getVariable());

    QDialog::accept();
    component->reload();
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
    static int timerId = -1;
    addVariableEditor({});
    onChanged();

    if (timerId != -1)
    {
        killTimer(timerId);
        timerId = -1;
    }
    timerId = startTimer(150);
}

void ComponentEditor::on_btnRemoveVariable_clicked()
{
    if (focusedEditor)
    { // Remove the corresponding editor
        variableEditors.removeOne(focusedEditor);
        ui->variablesView->layout()->removeWidget(focusedEditor);
        delete focusedEditor;
    }

    focusedEditor = nullptr;
    changed = true;

    updateDesignatorFields();
}

void ComponentEditor::on_editFunction_textEdited(const QString &text)
{
    Q_UNUSED(text)
    updateDesignatorVariable();
    changed = true;
}

void ComponentEditor::on_editLocation_textEdited(const QString &text)
{
    Q_UNUSED(text)
    updateDesignatorVariable();
    changed = true;
}

void ComponentEditor::on_editDesignation_textEdited(const QString &text)
{
    Q_UNUSED(text)
    updateDesignatorVariable();
    changed = true;
}
