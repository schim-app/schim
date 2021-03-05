#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>
#include <QSet>

struct Variable;

typedef QList<Variable> VariableSet;

/**
 * @brief A model representation of a variable.
 *
 * Every variable has three fields: `names`, a `value` and a `description`.
 *
 * The StringList `names` is used to identify the variable in a string. Each
 * string in `names` is an alias for the variable, and must match
 * `allowedPatterns`.
 *
 * The `value` is what the variable will be replaced with when displaying text.
 *
 * The `description` is simply a way for the user to document the variable.
 */
struct Variable
{
    // PUBLIC ATTRIBUTES
    /**
     * @brief Contains all names that can be used to identify this variable,
     * separated by a comma.
     */
    QStringList names;
    QString value, description;

    // CONSTRUCTORS
    Variable() = default;
    Variable(const QStringList &names, const QString &value = "");
    Variable(const QStringList &names, const QString &value, const QString
            &description);

    // GETTERS
    QString getTrueName() const;
    QStringList getAliases() const;

    // STATIC
    /// @brief Return the regex pattern that represents a valid variable name.
    static QString allowedPatterns();
    /// @brief Find a variable named `name` inside `list` and return it.
    static Variable find(const VariableSet &list, QString name);
    /**
     * @brief Substitute all variables in `str` with their values.
     *
     * A variable is any substring in `str` of the forms `%%pattern` or
     * `%{pattern}`, where pattern is any pattern that matches against the regex
     * returned by `allowedPatterns`.
     *
     * @param variableSet Supplies the values for the variables. If a variable
     * occurs in `str` but is not contained in `variableSet`, it is assumed to
     * have an empty value.
     */
    static QString substitute(QString str, const VariableSet &variableSet);

    // OPERATORS
    bool operator==(const Variable &var) const;
};

#endif // VARIABLE_H
