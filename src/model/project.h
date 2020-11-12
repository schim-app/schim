#ifndef PROJECT_H
#define PROJECT_H

#include "sheet.h"

#include <QList>
#include <QString>

/**
 * @brief A project is essentially a list of sheets, along with some project properties.
 *
 * This class should always take care of the destruction of sheets.
 */
class Project : public QList<Sheet*>
{
    QString title{"Untitled Project"};

public:
    /** @brief Construct an empty project with default properties. */
    Project();
    Project(const QList<Sheet*> &sheets);

    ~Project();

    // GETTERS
    QString getTitle() const;
    VariableSet &getVariables();
    VariableSet getVariables() const;

    // SETTERS
    void setTitle(const QString &title);

private:
    VariableSet variables;
};

#endif // PROJECT_H
