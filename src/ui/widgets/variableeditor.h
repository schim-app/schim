#ifndef VARIABLEEDITOR_H
#define VARIABLEEDITOR_H

#include <QWidget>

#include "model/variable.h"

namespace Ui {
class VariableEditor;
}

class VariableEditor : public QWidget
{
    Q_OBJECT

public:
    explicit VariableEditor(const Variable &variable, QWidget *parent = nullptr);
    ~VariableEditor();

    void reload();

    Variable getVariable() const;

    bool eventFilter(QObject* obj, QEvent *event) override;

signals:

    void changed();
    void focused();

private:

    void updateVariableName();

private slots:

    void on_editName_textEdited(const QString &text);
    void on_editAbbreviations_textEdited(const QString &text);
    void on_editValue_textEdited(const QString &text);
    void on_editDescription_textEdited(const QString &text);

private:
    Ui::VariableEditor *ui;
    Variable variable;
};

#endif // VARIABLEEDITOR_H
