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
     * @brief Create a scene operation in the specified scene.
     */
    SceneOperation(SheetScene *scene);
    virtual ~SceneOperation();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void cancel();

signals:
    void finished();

protected:
    SheetScene *scene{};
    GObject *obj{};
    int state = 0;
};

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

class OpInsertRect : public SceneOperation
{
public:
    using SceneOperation::SceneOperation;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    GRect *object() const;
};

class OpInsertText : public SceneOperation
{
    Q_OBJECT
public:
    OpInsertText(SheetScene *scene, GText *text = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /// Convenience function to cast obj
    GText *object() const;
};

class OpInsertComponent : public SceneOperation
{
public:
    OpInsertComponent(SheetScene *scene, Object *obj);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // OPERATION_H
