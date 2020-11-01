#include "variable.h"

Variable::Variable() { }

Variable::Variable(const QString &name, const QString &value)
    : name(name), value(value)
{ }

Variable::Variable(const QString &name, const QString &description, const QString &value)
    : name(name), description(description), value(value)
{ }
