#ifndef PROJECT_H
#define PROJECT_H

#include "sheet.h"

#include <QList>
#include <QMap>
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
    explicit Project(const QList<Sheet*> &sheets);
    ~Project();

    // GETTERS
    QString getProperty(const QString name);
    QList<Sheet *> &getSheets();
    QList<Sheet *> getSheets() const;
    /**
     * @brief A project is an entity that has no parent.
     */
    virtual Entity *getParent() const = delete;

    // SETTERS
    void setProperty(const QString &name, const QString &value);
    void addSheet(Sheet *sheet, int index=-1);
    void removeSheet(Sheet *sheet);
    /**
     * @brief A project is an entity that has no parent.
     */
    virtual void setParent() const = delete;

    // MISCELLANEOUS
    QList<Sheet *>::iterator begin();
    QList<Sheet *>::iterator end();

    // OPERATORS
    Project &operator=(Project &&project);

private:
    // ATTRIBUTES
    QList<Sheet *> sheets;
    /**
     * Contains all textual project properties, except for the name -- the name
     * is contained inside the base class `Entity`
     */
    // TODO provide better integration with variables
    QMap<QString, QString> properties {
        {"author", ""},
        {"number", ""},
        {"revision", ""},
        {"standard", ""},
    };
};

#endif // PROJECT_H
