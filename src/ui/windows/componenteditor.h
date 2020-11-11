#ifndef COMPONENTEDITOR_H
#define COMPONENTEDITOR_H

#include <QDialog>

#include "model/component.h"
#include "ui/widgets/variableeditor.h"

namespace Ui { class ComponentEditor; }

class ComponentEditor : public QDialog
{
public:
    ComponentEditor(Component *component, QWidget *parent = nullptr);
    ~ComponentEditor();

private:

    /** @brief Reload the widget with data from the component */
    void reload();

private slots:

    void onChanged();

    void accept() override;
    void reject() override;

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::ComponentEditor *ui;
    Component *component;
    QList<VariableEditor*> variableEditors;
    bool changed = false;
};

#endif // COMPONENTEDITOR_H
