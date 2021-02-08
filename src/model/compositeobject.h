#ifndef COMPOSITEOBJECT_H
#define COMPOSITEOBJECT_H

#include "object.h"
#include "model/variable.h"

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

public:
    CompositeObject();
    /** @brief Copy constructor */
    CompositeObject(const CompositeObject &obj);
    ~CompositeObject();

    Object *clone() const override;

    // GETTERS
    QPointF getPos() const override;
    /**
     * @brief Return the value of the variable with the specified name.
     *
     * @param name The name of the variable.
     * @param exists `true` is written to this variable if the variable is found,
     * and `false` otherwise.
     */
    QString getValue(const QString &name, bool *exists = nullptr) const;
    VariableSet getVariables() const;
    VariableSet &getLocalVariables();
    VariableSet getLocalVariables() const;
    QString getSourceFile() const;

    // SETTERS
    void setPos(const QPointF &pos) override;
    void setValue(const QString &name, const QString &value);
    void setLocalVariables(const VariableSet &vars);
    void setSourceFile(const QString &filename);

    // MODIFIER METHODS
    void addVariable(const Variable &variable);
    void append(Object *object);
    void append(const QList<Object*> &list);

    // OPERATORS
    bool operator==(const CompositeObject &obj) const;
    bool operator!=(const CompositeObject &obj) const;

protected:
    // TODO determine appropriate container
    VariableSet variables;
    QPointF pos{};
    // If != "", the object is always taken from the specified file
    QString file;

    friend class GCompositeObject;

};

#endif // COMPOSITEOBJECT_H
