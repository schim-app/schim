#ifndef PROJECT_H
#define PROJECT_H

#include "sheet.h"

#include <QList>
#include <QString>

/**
 * @brief A project is essentially a list of sheets, along with some project
 * properties.
 *
 * This class should always take care of the destruction of sheets.
 */
class Project
{
public:
    // CONSTRUCTORS
    /// @brief Construct an empty project with default properties.
    Project() = default;
    Project(const QList<Sheet*> &sheets);
    ~Project();

    // GETTERS
    QString getTitle() const;
    VariableSet &getVariables();
    VariableSet getVariables() const;
    QList<Sheet *> &getSheets();
    QList<Sheet *> getSheets() const;

    // SETTERS
    void setTitle(const QString &title);
    void addSheet(Sheet *sheet);
    void removeSheet(Sheet *sheet);

    // MISCELLANEOUS
    QList<Sheet *>::iterator begin();
    QList<Sheet *>::iterator end();

private:
    // ATTRIBUTES
    QString title{"Untitled Project"};
    VariableSet variables;
    QList<Sheet *> sheets;
};

#endif // PROJECT_H
