#include "operations.h"
#include "commands.h"

#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>

Operation::Operation(SheetScene *scene)
    : scene(scene) {
    connect(this, &Operation::testSig, this, [this](){});
}

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
    scene->operationFinished();
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

TextInsertOperation::TextInsertOperation(SheetScene *scene)
    : Operation(scene)
{
}

void TextInsertOperation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && state == 0)
    {
        auto pos = scene->getSnappedCursorPos();
        obj = new GText;
        obj->get()->setPos(pos);
        obj->reload();
        scene->addItem(obj);
        object()->setEditMode(true);

        // We declare these as local variables so we can capture them inside the lambda
        GText *obj = object();
        auto *scene = this->scene;

        // On unfocus, either:
        // - remove the item from the scene if it is left empty
        // - insert the item into the model if it has text in it
        connect(object(), &GText::focusOut, [scene, obj]() {
            if (obj->get()->getText().isEmpty())
            {
                scene->removeItem(obj);
                delete obj->get();
                obj->deleteLater();
            }
            else
                scene->command(new CmdInsertObject(obj, scene));
        });
        scene->operationFinished();
        state = 1;
    }
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
        obj->apply();
        scene->command(new CmdInsertObject(obj, scene));
        scene->operationFinished();
    }
}

void ComponentInsertOperation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    scene->setSnapCursorGuides(true);
    obj->setPos(scene->getSnappedCursorPos());
}
