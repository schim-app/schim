#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>

struct Variable
{
    QString name, description, value;

    Variable();
    Variable(const QString &name, const QString &value = "");
    Variable(const QString &name, const QString &description, const QString &value = "");
};

#endif // VARIABLE_H
