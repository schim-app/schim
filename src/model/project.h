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
class Project : public Entity
{
public:
    // CONSTRUCTORS
    /// @brief Construct an empty project with default properties.
    Project();
    Project(const QList<Sheet*> &sheets);
    ~Project();

    // GETTERS
    VariableSet &getVariables();
    VariableSet getVariables() const;
    QList<Sheet *> &getSheets();
    QList<Sheet *> getSheets() const;

    // SETTERS
    void addSheet(Sheet *sheet, int index=-1);
    void removeSheet(Sheet *sheet);

    // MISCELLANEOUS
    QList<Sheet *>::iterator begin();
    QList<Sheet *>::iterator end();

private:
    // ATTRIBUTES
    VariableSet variables;
    QList<Sheet *> sheets;
};

#endif // PROJECT_H
