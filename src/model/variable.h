#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>
#include <QSet>

struct Variable;

typedef QList<Variable> VariableSet;

struct Variable
{
    QString name, description, value;

    Variable();
    Variable(const QString &name, const QString &value = "");
    Variable(const QString &name, const QString &description, const QString &value);

    static QRegExp allowedPatterns();
    static Variable find(const VariableSet &list, QString name);
    static QString substitute(QString str, const VariableSet &variableSet);
};


#endif // VARIABLE_H
