#ifndef SHEET_H
#define SHEET_H

#include "header.h"
#include "object.h"

class Project;

/**
 * @brief This class models a sheet of paper.
 *
 * This class should always take care of the destruction of the objects it
 * contains. It inherits QList<Object*> so that we don't have to reimplement
 * convenience functions that QList already provides.
 */
class Sheet
{
public:
    // CONSTRUCTORS
    Sheet(bool defaultHeader = true);
    ~Sheet();

    // GETTERS
    float getWidth() const;
    float getHeight() const;
    QString getTitle() const;
    Header *getHeader();
    QRectF getContentArea() const;
    QList<Object *> &getObjects();
    QList<Object *> getObjects() const;
    VariableSet &getLocalVariables();
    VariableSet getLocalVariables() const;
    VariableSet getVariables() const;

    // SETTERS
    void setWidth(float width);
    void setHeight(float height);
    void setHeight() const;
    void setProject(Project *project);
    void setTitle(const QString &title);
    /**
     * @brief Set this sheet's header.
     *
     * The sheet becomes the owner of `header` and is responsible for its
     * deletion when the sheet's destructor is called.
     *
     * @param destroy Whether to destroy the old header.
     */
    void setHeader(Header *header, bool destroy = true);
    void setLocalVariables(const VariableSet &vars);
    void addVariable(const Variable &variable);

    void addObject(Object *obj);
    void removeObject(Object *obj);
    QList<Object *>::iterator begin();
    QList<Object *>::iterator end();

private:
    // ATTRIBUTES
    float width = 297, height = 210;
    Header *header = nullptr;
    QString title{"New sheet"};

    VariableSet variables;
    QList<Object *> objects;
    Project *project{};

    friend class Project;
};

#endif // SHEET_H
