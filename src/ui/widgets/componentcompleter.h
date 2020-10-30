#ifndef COMPONENTCOMPLETER_H
#define COMPONENTCOMPLETER_H

#include <QLineEdit>

class ComponentCompleter : public QLineEdit
{
public:
    ComponentCompleter(QWidget *parent = nullptr);

private:
    // EVENTS
    void focusOutEvent(QFocusEvent *event) override;
};

#endif // COMPONENTCOMPLETER_H
