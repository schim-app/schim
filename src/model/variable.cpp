#include "variable.h"

Variable::Variable() { }

Variable::Variable(const QString &name, const QString &value)
    : name(name), value(value)
{ }

Variable::Variable(const QString &name, const QString &value, const QString &description)
    : name(name), value(value), description(description)
{ }

QString Variable::allowedPatterns()
{
    //TODO document
    return "[A-Za-z_]{1,}";
}

Variable Variable::find(const VariableSet &list, QString name)
{
    QRegExp pattern("%" + allowedPatterns());
    //TODO document
    if (!(pattern.exactMatch("%" + name) || pattern.exactMatch(name)))
        return {}; // Invalid name format

    name.replace("%", "");
    foreach (auto var, list)
    {
        // Match the name against all aliases of var
        foreach (auto alias, var.name.split(" "))
            if (name == alias)
                return var;
    }

    return {};
}

QString Variable::substitute(QString str, const VariableSet &variableSet)
{
    QRegExp pattern1("%" + allowedPatterns()),
            pattern2("%\\{" + allowedPatterns() + "\\}");

    str.replace("%%", QString() + char(1));

    // Search for %pattern or %{pattern} in `str`
    for(;;)
    {
        QRegExp matchedPattern;
        if (pattern1.indexIn(str) != -1)
            matchedPattern = pattern1;
        else if (pattern2.indexIn(str) != -1)
            matchedPattern = pattern2;
        else
            break;
        // Replace the variable name with its contents
        QStringList captured = matchedPattern.capturedTexts();
        if (!captured.empty())
            str.replace(captured[0], Variable::find(variableSet, captured[0]).value);
    }

    str.replace(QString() + char(1), "%");

    return str;
}
