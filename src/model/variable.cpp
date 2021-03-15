#include "variable.h"

// CONSTRUCTORS
Variable::Variable(const QStringList &names, const QString &value)
    : names(names), value(value)
{ }

Variable::Variable(const QStringList &names, const QString &value, const QString &description)
    : names(names), value(value), description(description)
{ }

// GETTERS

QString Variable::getTrueName() const
{
    return names.empty() ? "" : names[0];
}

QStringList Variable::getAliases() const
{
    return names.mid(1);
}

// STATIC

QString Variable::allowedPatterns()
{
    return "[A-Za-z_]+";
}

Variable Variable::find(const VariableSet &list, QString name)
{
    QRegExp pattern("%" + allowedPatterns());
    // Check if `name` is a valid variable identifier
    if (!(pattern.exactMatch("%" + name) || pattern.exactMatch(name)))
        return {}; // Invalid name format

    name.replace("%", "");
    foreach (auto var, list)
    {
        // Match the name against all aliases of var
        foreach (auto alias, var.names)
            if (name == alias)
                return var;
    }

    return {};
}

QString Variable::substitute(QString str, const VariableSet &variableSet)
{
    QRegExp pattern1("%" + allowedPatterns()),
            pattern2("%\\{" + allowedPatterns() + "\\}");

    // Replace %% with a non printable character so it doesn't match against a variable
    str.replace("%%", QString() + char(1));

    // Search for %pattern or %{pattern} in `str`
    for (;;)
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
        {
            // Remove {} if second pattern was matched
            QRegExp pat(allowedPatterns());
            pat.indexIn(captured[0]);
            str.replace(captured[0],
                    Variable::find(variableSet, pat.capturedTexts().at(0)).value);
        }
    }

    str.replace(QString() + char(1), "%");

    return str;
}

// OPERATORS

bool Variable::operator==(const Variable &var) const
{
    return getTrueName() == var.getTrueName();
}
