#ifndef COMPONENTEDITOR_H
#define COMPONENTEDITOR_H

#include <QDialog>

namespace Ui { class ComponentEditor; }

class ComponentEditor : public QDialog
{
public:
    ComponentEditor(QWidget *parent = nullptr);
    ~ComponentEditor();

private:
    Ui::ComponentEditor *ui;
};

#endif // COMPONENTEDITOR_H
