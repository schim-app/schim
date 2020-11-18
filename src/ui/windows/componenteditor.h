#ifndef COMPONENTEDITOR_H
#define COMPONENTEDITOR_H

#include <QDialog>

#include "model/component.h"
#include "ui/objects/gcomponent.h"
#include "ui/widgets/variableeditor.h"

namespace Ui { class ComponentEditor; }

class ComponentEditor : public QDialog
{
public:
    ComponentEditor(GComponent *component, QWidget *parent = nullptr);
    ~ComponentEditor();

private:

    /** @brief Reload the widget with data from the component */
    void reload();

    void addVariableEditor(const Variable &var, int index = -1);

    QString compileDesignator();
    void updateDesignatorVariable();
    void updateDesignatorFields();

    void timerEvent(QTimerEvent *event) override;

private slots:

    void onChanged();
    void onChildFocused();

    void accept() override;
    void reject() override;

    void on_btnAddVariable_clicked();
    void on_btnRemoveVariable_clicked();
    void on_editFunction_textEdited(const QString &text);
    void on_editLocation_textEdited(const QString &text);
    void on_editDesignation_textEdited(const QString &text);

private:
    Ui::ComponentEditor *ui;
    GComponent *component;
    QList<VariableEditor*> variableEditors;
    VariableEditor *focusedEditor{};
    bool changed = false;
};

#endif // COMPONENTEDITOR_H
