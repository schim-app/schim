/// @file operations.h
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
class SceneOperation : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Construct a scene operation in the specified scene.
     */
    SceneOperation(SheetScene *scene);
    virtual ~SceneOperation();

    /**
     * @brief Process a mousePressEvent.
     * The base implementation does nothing.
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /**
     * @brief Process a mouseMoveEvent.
     * The base implementation does nothing.
     */
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    /**
     * @brief Process a mouseReleaseEvent.
     * The base implementation does nothing.
     */
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    /**
     * @brief Process a keyPressEvent.
     * The base implementation does nothing.
     */
    virtual void keyPressEvent(QKeyEvent *event);
    /**
     * @brief Cancel the operation.
     *
     * This will delete any dangling objects and remove them from the scene.
     * The `finished` signal will be emitted.
     */
    virtual void cancel();

signals:
    /**
     * @brief Emitted when the operation has finished, either successfully or not.
     */
    void finished();

protected:
    SheetScene *scene{};
    GObject *obj{};
    int state = 0;
};

/**
 * @brief A scene operation that is triggered when the user inserts a line.
 */
class OpInsertLine : public SceneOperation
{
public:
    using SceneOperation::SceneOperation;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    /// Convenience function to cast obj
    GLine *object() const;
};

/**
 * @brief A scene operation that is triggered when the user inserts a rectangle.
 */
class OpInsertRect : public SceneOperation
{
public:
    using SceneOperation::SceneOperation;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    GRect *object() const;
};

/**
 * @brief A scene operation that is triggered when the user inserts a text.
 */
class OpInsertText : public SceneOperation
{
    Q_OBJECT
public:
    OpInsertText(SheetScene *scene, GText *text = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /// Convenience function to cast obj
    GText *object() const;
};

/**
 * @brief A scene operation that is triggered when the user inserts a component.
 */
class OpInsertComponent : public SceneOperation
{
public:
    OpInsertComponent(SheetScene *scene, Object *obj);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // OPERATION_H
