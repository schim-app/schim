#ifndef SHEET_H
#define SHEET_H

#include "entity.h"
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
class Sheet : public Entity
{
public:
    // CONSTRUCTORS
    Sheet(bool defaultHeader = true);
    ~Sheet();

    // GETTERS
    float getWidth() const;
    float getHeight() const;
    Header *getHeader();
    QRectF getContentArea() const;
    QList<Object *> &getObjects();
    QList<Object *> getObjects() const;
    /**
     * @brief Return the index of the sheet in the project it belongs to.
     */
    int getIndex();
    /**
     * @brief Override `getParent` with covariant return type `Project`.
     */
    virtual Project *getParent() const;

    // SETTERS
    void setWidth(float width);
    void setHeight(float height);
    void setHeight() const;
    /**
     * @brief Set this sheet's header.
     * @param destroy Whether to destroy the old header.
     * @note The sheet takes ownership of `header`.
     */
    void setHeader(Header *header, bool destroy = true);
    void addObject(Object *obj);
    void removeObject(Object *obj);
    virtual void setParent(Project *project);

    // MISCELLANEOUS
    QList<Object *>::iterator begin();
    QList<Object *>::iterator end();

private:
    // ATTRIBUTES
    float width = 297, height = 210;
    Header *header = nullptr;

    VariableSet variables;
    QList<Object *> objects;
};

#endif // SHEET_H
