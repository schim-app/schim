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
        if (obj->scene())
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
        auto pos = scene->getCursorPos();
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
            scene->removeItem(obj); // Remove from scene to add it via command
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
        object()->get()->setP2(scene->getCursorPos());
        obj->reloadFromModel();
        // TODO performance??
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
        auto pos = scene->getCursorPos();
        if (state == 0)
        { // Inserting the first corner
            obj = new GRect; // Create the object
            // Set model properties
            object()->get()->setRect(pos.x(), pos.y(), 0, 0);
            obj->reloadFromModel();
            scene->addItem(obj); // Add to scene
            ++state;
        }
        else if (state == 1)
        { // Inserting the other corner
            // Set model properties
            object()->get()->setBottomRight(pos);
            obj->reloadFromModel();
            scene->removeItem(obj); // Remove from scene to add it via command
            // Register the insertion as a command
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
    { // The first point has been placed; waiting for second point
        // The rect is updated in real time
        object()->get()->setBottomRight(scene->getCursorPos());
        obj->reloadFromModel();
        // TODO performance??
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
    this->obj = (text != nullptr ? text : new GText);
}

void OpInsertText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && state == 0)
    {
        auto pos = scene->getCursorPos();
        // Set the position of the text item
        obj->setPos(obj->mapFromScene(pos));
        if (!obj->parentItem()) // Add it to the scene if it is parent-less
            scene->addItem(obj);
        object()->setEditMode(true); // Initiate edit mode

        // We declare these as local variables to capture them inside the lambda
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
                // Register the insertion as a command
                scene->command(new CmdInsertObject(obj, scene, obj->parentItem()));
        });
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
    this->obj->setPos(scene->getCursorPos());
}

void OpInsertComponent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto pos = scene->getCursorPos();
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
    obj->setPos(scene->getCursorPos());
}
