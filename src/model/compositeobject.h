#ifndef COMPOSITEOBJECT_H
#define COMPOSITEOBJECT_H

#include "object.h"
#include "model/variable.h"

#include <QList>

/**
 * @brief An object that consists of other child objects.
 *
 * This is the most general representation of an object. Destruction
 * of its child objects should always be performed in the destructor.
 * This class inherits QList<Object*> so we don't have to
 * reimplement the convenience functions that QList already provides.
 */
class CompositeObject : public Object, public QList<Object*>
{

protected:

public:
    CompositeObject();
    /** @brief Copy constructor */
    CompositeObject(const CompositeObject &obj);

    ~CompositeObject();

    // GETTERS
    /**
     * @brief Return the value of the variable with the specified name.
     *
     * @param name The name of the variable.
     * @param exists `true` is written to this variable if the variable is found,
     * and `false` otherwise.
     */
    QString getValue(const QString &name, bool *exists) const;
    QList<Variable> getVariables() const;

    // SETTERS
    void setValue(const QString &name, const QString &value);
    void addVariable(const Variable &variable);

    Object *clone() const override;

    friend class GCompositeObject;

private:
    // TODO determine appropriate container
    QList<Variable> variables;
};

#endif // COMPOSITEOBJECT_H
