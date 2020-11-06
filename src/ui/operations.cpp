#include "operations.h"
#include "commands.h"

#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>

Operation::Operation(SheetScene *scene)
    : scene(scene) { }

Operation::~Operation() { }

void Operation::mousePressEvent(QGraphicsSceneMouseEvent *) { }

void Operation::mouseMoveEvent(QGraphicsSceneMouseEvent *) { }

void Operation::mouseReleaseEvent(QGraphicsSceneMouseEvent *) { }

void Operation::keyPressEvent(QKeyEvent *) { }

void Operation::cancel()
{
    if (obj)
    {
        scene->removeItem(obj);
        delete obj->get();
        delete obj;
    }
    scene->operationFinished(false);
}

//////////////////////////////////////////////////////////////////////////////////

void LineInsertOperation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto pos = scene->getSnappedCursorPos();
        if (state == 0) // Waiting for first point to be placed
        {
            obj = new GLine;
            object()->get()->setLine(pos.x(), pos.y(), pos.x(), pos.y());
            obj->reload();
            scene->addItem(obj);
            ++state;
        }
        else if (state == 1) // The first point has been inserted
        {
            object()->get()->setP2(pos);
            obj->reload();
            scene->removeItem(obj);
            scene->command(new CmdInsertObject(obj, scene));
            state = 2; // Finished state
            scene->operationFinished();
        }
    }
}

void LineInsertOperation::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
    scene->setSnapCursorGuides(true);
    if (state == 1)
    {
        object()->get()->setP2(scene->getSnappedCursorPos());
        obj->reload();
        scene->update();
    }
}

GLine *LineInsertOperation::object() const
{
    return (GLine *) obj;
}

//////////////////////////////////////////////////////////////////////////////////

void RectInsertOperation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto pos = scene->getSnappedCursorPos();
        if (state == 0)
        {
            obj = new GRect;
            object()->get()->setRect(pos.x(), pos.y(), 0, 0);
            obj->reload();
            scene->addItem(obj);
            ++state;
        }
        else if (state == 1)
        {
            object()->get()->setBottomRight(pos);
            obj->reload();
            scene->removeItem(obj);
            scene->command(new CmdInsertObject(obj, scene));
            state = 2;
            scene->operationFinished();
        }
    }
}

void RectInsertOperation::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
    scene->setSnapCursorGuides(true);
    if (state == 1)
    {
        object()->get()->setBottomRight(scene->getSnappedCursorPos());
        obj->reload();
        scene->update();
    }
}

GRect *RectInsertOperation::object() const
{
    return (GRect*) obj;
}

//////////////////////////////////////////////////////////////////////////////////

void TextInsertOperation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto pos = scene->getSnappedCursorPos();
        obj = new GText;
        obj->get()->setPos(pos);
        obj->reload();
        scene->addItem(obj);
        object()->setEditable();
        //TODO move this somewhere else. Make it so that when the text item is
        // unfocused, detect if the text is empty and remove the item.
        // This would require a signal in GText but right now I can't implement
        // it because of multiple inheritance.
        scene->command(new CmdInsertObject(obj, scene));
        scene->operationFinished();
    }
}

void TextInsertOperation::textItemUnfocused()
{
    scene->operationFinished();
}

GText *TextInsertOperation::object() const
{
    return (GText*) obj;
}

//////////////////////////////////////////////////////////////////////////////////

ComponentInsertOperation::ComponentInsertOperation(SheetScene *scene, Object *obj)
    : Operation(scene)
{
    this->obj = GObject::assign(obj);
    scene->addItem(this->obj);
    this->obj->setPos(scene->getSnappedCursorPos());
}

void ComponentInsertOperation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto pos = scene->getSnappedCursorPos();
        obj->setPos(pos);
        obj->apply(); // TODO Implement this method
        scene->command(new CmdInsertObject(obj, scene));
        scene->operationFinished();
    }
}

void ComponentInsertOperation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    scene->setSnapCursorGuides(true);
    obj->setPos(scene->getSnappedCursorPos());
}
