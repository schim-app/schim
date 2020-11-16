#include "componentcompleter.h"

ComponentCompleter::ComponentCompleter(QWidget *parent)
    : QLineEdit(parent)
{
}

void ComponentCompleter::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    hide();
}
