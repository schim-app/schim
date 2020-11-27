#include "variable.h"

Variable::Variable() { }

Variable::Variable(const QString &name, const QString &value)
    : name(name), value(value)
{ }

Variable::Variable(const QString &name, const QString &value, const QString &description)
    : name(name), value(value), description(description)
{ }

QRegExp Variable::allowedPatterns()
{
    //TODO document
    return QRegExp("%[A-Za-z_]{1,}");
}

Variable Variable::find(const VariableSet &list, QString name)
{
    //TODO document
    if (!(allowedPatterns().exactMatch("%" + name) || allowedPatterns().exactMatch(name)))
        return {}; // Invalid name format

    name.replace("%", "");
    for (auto var : list)
    {
        // Match the name against all aliases of var
        for (auto alias : var.name.split(" "))
            if (name == alias)
                return var;
    }

    return {};
}

QString Variable::substitute(QString str, const VariableSet &variableSet)
{
    QRegExp pattern(allowedPatterns());

    str.replace("%%", QString() + char(1));

    while (pattern.indexIn(str) != -1)
    {
        QStringList captured = pattern.capturedTexts();
        if (!captured.empty())
            str.replace(captured[0], Variable::find(variableSet, captured[0]).value);
    }

    str.replace(QString() + char(1), "%");

    return str;
}
