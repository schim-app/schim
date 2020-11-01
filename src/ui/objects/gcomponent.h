#ifndef GCOMPONENT_H
#define GCOMPONENT_H

#include "model/component.h"
#include "ui/objects/gcompositeobject.h"

#include <QObject>

class GComponent : public GCompositeObject
{
    Q_OBJECT
public:
    GComponent(Component *obj);

    Component *get();
    const Component *get() const;

    void showContextMenu();

    // EVENTS
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
    void edit();
};

#endif // GCOMPONENT_H
