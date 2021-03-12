#include "operations.h"
#include "commands.h"

#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>

SceneOperation::SceneOperation(SheetScene *scene)
    : scene(scene)
{
}

SceneOperation::~SceneOperation() { }

void SceneOperation::mousePressEvent(QGraphicsSceneMouseEvent *) { }

void SceneOperation::mouseMoveEvent(QGraphicsSceneMouseEvent *) { }

void SceneOperation::mouseReleaseEvent(QGraphicsSceneMouseEvent *) { }

void SceneOperation::keyPressEvent(QKeyEvent *) { }

void SceneOperation::cancel()
{
    if (obj)
    {
        scene->removeItem(obj);
        delete obj->get();
        delete obj;
    }
    emit finished();
}

//////////////////////////////////////////////////////////////////////////////////

void OpInsertLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto pos = scene->getSnappedCursorPos();
        if (state == 0)
        { // Insertion of first point
            obj = new GLine; // Create line
            // Set model properties
            object()->get()->setLine(pos.x(), pos.y(), pos.x(), pos.y());
            obj->reloadFromModel();
            scene->addItem(obj); // Add to scene
            ++state;
        }
        else if (state == 1)
        { // Insertion of second point
            object()->get()->setP2(pos); // Set model properties
            obj->reloadFromModel();
            // Remove the object from the scene - it will be added again via command
            scene->removeItem(obj);
            // Register the insertion as a command
            scene->command(new CmdInsertObject(obj, scene));
            emit finished();
        }
    }
}

void OpInsertLine::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
    scene->setSnapCursorGuides(true);
    if (state == 1)
    { // The first point has already been placed into the scene
        // The line is updated in real time
        object()->get()->setP2(scene->getSnappedCursorPos());
        obj->reloadFromModel();
        scene->update(object()->boundingRect().translated(object()->scenePos()));
    }
}

GLine *OpInsertLine::object() const
{
    return (GLine *) obj;
}

//////////////////////////////////////////////////////////////////////////////////

void OpInsertRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto pos = scene->getSnappedCursorPos();
        if (state == 0)
        {
            obj = new GRect;
            object()->get()->setRect(pos.x(), pos.y(), 0, 0);
            obj->reloadFromModel();
            scene->addItem(obj);
            ++state;
        }
        else if (state == 1)
        {
            object()->get()->setBottomRight(pos);
            obj->reloadFromModel();
            scene->removeItem(obj);
            scene->command(new CmdInsertObject(obj, scene));
            state = 2;
            emit finished();
        }
    }
}

void OpInsertRect::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
    scene->setSnapCursorGuides(true);
    if (state == 1)
    {
        object()->get()->setBottomRight(scene->getSnappedCursorPos());
        obj->reloadFromModel();
        scene->update();
    }
}

GRect *OpInsertRect::object() const
{
    return (GRect*) obj;
}

//////////////////////////////////////////////////////////////////////////////////

OpInsertText::OpInsertText(SheetScene *scene, GText *text)
    : SceneOperation(scene)
{
    this->obj = text;
}

void OpInsertText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && state == 0)
    {
        auto pos = scene->getSnappedCursorPos();
        if (obj == nullptr)
            obj = new GText;
        // We are doing this so it displays variables correctly

        /* TODO determine what to do
        obj->get()->setParent(scene->getSheet());
        obj->get()->setParent(obj->parentItem()->get());
        */
        // Position the object relative to its parent (if it has one)
        if (obj->parentItem() == nullptr)
            obj->get()->setPos(pos);
        else
            obj->get()->setPos(obj->parentItem()->mapFromScene(pos));
        obj->reloadFromModel(); // Update the object visually
        if (!obj->parentItem()) // Add it to the scene if it is parent-less
            scene->addItem(obj);
        object()->setEditMode(true); // Initiate edit mode

        // We declare these as local variables so we can capture them inside the lambda
        GText *obj = object();
        auto *scene = this->scene;

        // On unfocus, either:
        // - remove the item from the scene if it is empty
        // - insert the item into the model if text has been entered
        connect(object(), &GText::focusOut, [scene, obj]() {
            if (obj->get()->getText().isEmpty())
            {
                scene->removeItem(obj);
                delete obj->get();
                obj->deleteLater();
            }
            else
                scene->command(new CmdInsertObject(obj, scene, obj->parentItem()));
        });
        state = 1;
        emit finished(); // Notify interested parties the operation is done.
    }
}

GText *OpInsertText::object() const
{
    return (GText*) obj;
}

//////////////////////////////////////////////////////////////////////////////////

OpInsertComponent::OpInsertComponent(SheetScene *scene, Object *obj)
    : SceneOperation(scene)
{
    this->obj = GObject::assign(obj);
    scene->addItem(this->obj);
    this->obj->setPos(scene->getSnappedCursorPos());
}

void OpInsertComponent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto pos = scene->getSnappedCursorPos();
        obj->setPos(pos);
        obj->applyToModel();
        scene->command(new CmdInsertObject(obj, scene));
        emit finished();
    }
}

void OpInsertComponent::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    scene->setSnapCursorGuides(true);
    obj->setPos(scene->getSnappedCursorPos());
}
