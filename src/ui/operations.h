#ifndef OPERATION_H
#define OPERATION_H

#include "ui/sheetscene.h"
#include "ui/objects/gobject.h"
#include "ui/objects/gline.h"
#include "ui/objects/grect.h"

class QGraphicsSceneMouseEvent;
class QKeyEvent;

class Operation
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
    SheetScene *scene;
    GObject *obj{};
};

class LineInsertOperation : public Operation
{
public:
    using Operation::Operation;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

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

#endif // OPERATION_H
