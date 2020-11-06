#ifndef OPERATION_H
#define OPERATION_H

#include "ui/sheetscene.h"
#include "ui/objects/gobject.h"
#include "ui/objects/gline.h"
#include "ui/objects/grect.h"
#include "ui/objects/gtext.h"

class QGraphicsSceneMouseEvent;
class QKeyEvent;

/**
 * @brief The base class of all scene operations.
 *
 * An operation is any multi-step process that relies on mouse/keyboard
 * input. An example of an operation is inserting a line.
 *
 * Each operation is associated with a scene.
 */
class Operation : public QObject
{
public:
    /**
     * @brief Create a scene operation in the specified scene.
     */
    Operation(SheetScene *scene);
    virtual ~Operation();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void cancel();

protected:
    SheetScene *scene{};
    GObject *obj{};
};

class LineInsertOperation : public Operation
{
public:
    using Operation::Operation;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    /// Convenience function to cast obj
    GLine *object() const;

private:
    int state = 0;
};

class RectInsertOperation : public Operation
{
public:
    using Operation::Operation;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    GRect *object() const;
private:
    int state = 0;
};

class TextInsertOperation : public Operation
{
    Q_OBJECT
public:
    using Operation::Operation;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    /// Convenience function to cast obj
    GText *object() const;

//private slots:
    void textItemUnfocused();

private:
    int state = 0;
};

class ComponentInsertOperation : public Operation
{
public:
    ComponentInsertOperation(SheetScene *scene, Object *obj);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:

private:
    int state = 0;
};

#endif // OPERATION_H
