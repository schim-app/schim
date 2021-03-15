#ifndef COMPONENTEDITOR_H
#define COMPONENTEDITOR_H

#include <QDialog>

#include "model/component.h"
#include "ui/objects/gcomponent.h"
#include "ui/widgets/variableeditor.h"

namespace Ui { class ComponentEditor; }

class ComponentSettings : public QDialog
{
public:
    explicit ComponentSettings(GComponent *component, QWidget *parent = nullptr);
    ~ComponentSettings();

private:

    /** @brief Reload the widget with data from the component */
    void reload();

    QString compileDesignator();
    void updateDesignatorVariable();
    void updateDesignatorFields();

private slots:

    void onChanged();

    void accept() override;
    void reject() override;

    void on_editFunction_textEdited(const QString &text);
    void on_editLocation_textEdited(const QString &text);
    void on_editDesignation_textEdited(const QString &text);

private:
    Ui::ComponentEditor *ui;
    GComponent *component;
    bool changed = false;
};

#endif // COMPONENTEDITOR_H
