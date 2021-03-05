#ifndef GCOMPONENT_H
#define GCOMPONENT_H

#include "model/component.h"
#include "ui/objects/gcompositeobject.h"

#include <QObject>

class GComponent : public GCompositeObject
{
    Q_OBJECT
public:
    // CONSTRUCTORS
    GComponent(Component *obj);

    // GETTERS
    Component *get();
    const Component *get() const;

    // USER ACTIONS
    void showContextMenu();

private:
    // EVENTS
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

public slots:
    // SLOTS
    void edit();
    void addText();

private:
    Operation *operation;
};

#endif // GCOMPONENT_H
