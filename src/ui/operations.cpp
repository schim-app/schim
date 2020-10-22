#include "operations.h"
#include "commands.h"

#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>

Operation::Operation(SheetScene *scene)
    : scene(scene) { }

Operation::~Operation() { }

void Operation::mousePressEvent(QGraphicsSceneMouseEvent *event) { }

void Operation::mouseMoveEvent(QGraphicsSceneMouseEvent *event) { }

void Operation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) { }

void Operation::keyPressEvent(QKeyEvent *event) { }

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

void LineInsertOperation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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

void RectInsertOperation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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
}

void TextInsertOperation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

}

void TextInsertOperation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (state == 1 && event->buttons() == Qt::LeftButton)
    {
    }
}

GText *TextInsertOperation::object() const
{
    return (GText*) obj;
}

//////////////////////////////////////////////////////////////////////////////////
