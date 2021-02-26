#include "componentsettings.h"

#include "ui_componentsettings.h"

#include <QMessageBox>
#include <QScrollBar>

ComponentSettings::ComponentSettings(GComponent *component, QWidget *parent)
    : QDialog(parent), ui(new Ui::ComponentEditor), component(component)
{
    ui->setupUi(this);

    // Initialization

    ui->variableEditor->setVariables(component->get()->getLocalVariables());
    reload();
    // TODO Why do I have to connect this here when I have clicked "Go to slot" in QtDesigner? Maybe I have fixed this -- check?
    connect(ui->editFunction, &QLineEdit::textEdited, this, &ComponentSettings::on_editFunction_textEdited);
    connect(ui->editLocation, &QLineEdit::textEdited, this, &ComponentSettings::on_editLocation_textEdited);
    connect(ui->editDesignation, &QLineEdit::textEdited, this, &ComponentSettings::on_editDesignation_textEdited);
    connect(ui->variableEditor, &VariableEditor::changed, this, &ComponentSettings::onChanged);

    setStatusTip("class ComponentSettings in src/ui/windows/componentsettings.h");
}

ComponentSettings::~ComponentSettings()
{
    delete ui;
}

// UPDATE METHODS

void ComponentSettings::reload()
{
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
    VariableSet vars = ui->variableEditor->getVariables();
    bool found = false;
    for (auto &var : vars)
        if (var.names.contains("designator"))
        {
            found = true;
            var.value = compileDesignator();
        }
    if (!found)
        vars.push_front({{"designator", "DT", "d"}, compileDesignator(), "Component designation"});
    ui->variableEditor->setVariables(vars);
}

void ComponentSettings::updateDesignatorFields()
{
    Variable var = Variable::find(ui->variableEditor->getVariables(), "designator");
    QRegExp equal("=[^=+-]+"); // To parse the function (=...)
    QRegExp plus("\\+[^=+-]+"); // To parse the location (+...)
    QRegExp dash("-[^=+-]+"); // To parse the designation (-...)

    // Default is empty
    ui->editFunction->setText("");
    ui->editLocation->setText("");
    ui->editDesignation->setText("");

    if (equal.indexIn(var.value) != -1)
        ui->editFunction->setText(equal.capturedTexts().at(0).mid(1));
    if (plus.indexIn(var.value) != -1)
        ui->editLocation->setText(plus.capturedTexts().at(0).mid(1));
    if (dash.indexIn(var.value) != -1)
        ui->editDesignation->setText(dash.capturedTexts().at(0).mid(1));

    // There is only a component designation
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

void ComponentSettings::accept()
{
    component->get()->setLocalVariables(ui->variableEditor->getVariables());

    QDialog::accept();
    component->reload();
}

void ComponentSettings::reject()
{
    if (!changed ||
        QMessageBox::question(this, "Are you sure?", "Discard changes to this component?")
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
