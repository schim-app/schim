#ifndef VARIABLE_H
#define VARIABLE_H

#include <QString>
#include <QSet>

struct Variable;

typedef QList<Variable> VariableSet;

/**
 * @brief A model representation of a variable.
 *
 * Every variable has three fields: a `name`, a `value` and a
 * `description`.
 *
 * The `name` is used to identify the variable in a string,
 * which is replaced by its value when displaying text. It
 * can contain a single string matching `allowedPatterns`
 * or multiple such strings separated by whitespace. In the
 * latter case, each such string represents an alias for
 * the variable.
 *
 * The `value` is what the variable will be replaced with when
 * displaying text.
 *
 * The `description` is simply a way to document the variable.
 */
struct Variable
{
    /** Contains all names that can be used to identify this variable, separated by a comma. */
    QStringList names;
    QString value, description;

    Variable();
    Variable(const QStringList &names, const QString &value = "");
    Variable(const QStringList &names, const QString &value, const QString &description);

    QString getTrueName() const;
    QStringList getAliases() const;

    /** @brief Return the regex pattern that represents a valid variable name. */
    static QString allowedPatterns();
    /** @brief Find a variable named `name` inside `list` and return it. */
    static Variable find(const VariableSet &list, QString name);
    /**
     * @brief Substitute all variables in `str` with their values.
     *
     * A variable is any substring in `str` of the forms `%%pattern` or
     * `%{pattern}`, where pattern is any pattern that matches against
     * the regex returned by `allowedPatterns`.
     *
     * @param variableSet Supplies the values for the variables. If a
     * variable occurs in `str` but is not contained in `variableSet`,
     * it is assumed to have an empty value.
     */
    static QString substitute(QString str, const VariableSet &variableSet);
};

#endif // VARIABLE_H
