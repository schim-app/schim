#ifndef GCOMPONENT_H
#define GCOMPONENT_H

#include "model/component.h"
#include "ui/objects/gcompositeobject.h"

#include <QObject>

class TextInsertOperation; //TODO tm
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
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

public slots:
    void edit();
    void addText();

private:
    Operation *operation;
};

#endif // GCOMPONENT_H
