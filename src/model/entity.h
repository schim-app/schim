/// @file entity.h
#ifndef ENTITY_H
#define ENTITY_H

#include "variable.h"

/**
 * @brief Base class for entities.
 *
 * An entity is any class that has some metadata associated with it. Metadata
 * can be a name, annotative properties and variables. Currently, the only
 * entities in Schim are `Project`, `Sheet`, `CompositeObject` and any of their
 * subclasses.
 *
 * The currently defined common metadata for all entities are:
 *
 * - `name`, a friendly name. Can be empty but this is not recommended.
 * - `filename`, the source file of the entity. This can be empty.
 * - `variables`, various variables including built-in and user-defined ones.
 * - `parent`, the parent entity.
 *
 * @note Entities are not the only items that can have parents. Other items like
 * primitive objects also have parents, but they need not keep a reference to
 * their parent (to reduce memory usage). In some circumstances primitive objects
 * may keep a reference to their parent, but this is treated on a case-by-case
 * basis.
 * @note Children of the entity are not handled in the base class. Derived
 * classes should implement this on their own if needed.
 *
 * @see `Variable`
 */
class Entity
{
public:
    /**
     * @brief Construct an empty entity with an optional parent.
     */
    explicit Entity(Entity *parent = nullptr);
    /**
     * @brief Default destructor that makes this class virtual.
     */
    virtual ~Entity() = default;

    // GETTERS
    QString getName() const;
    QString getFileName() const;
    /**
     * @brief Return the variables defined for this entity.
     * @returns The variables defined for this entity, including local variables
     * and those inherited from the parent entity.
     */
    VariableSet getVariables() const;
    VariableSet &getLocalVariables();
    VariableSet getLocalVariables() const;
    Entity *getParent() const;

    // SETTERS
    void setName(const QString &name);
    void setFileName(const QString &name);
    void setVariables(const VariableSet &vars);
    void addVariable(const Variable &variable);
    void addVariables(const VariableSet &variables);
    void setParent(Entity *parent);

protected:
    /// The name of the entity.
    QString name;
    /// The name of the source file where the entity is defined.
    QString fileName;
    /// The variables associated with this entity.
    VariableSet variables;
    /// The parent of this entity.
    Entity* parent{};
};

#endif // ENTITY_H
