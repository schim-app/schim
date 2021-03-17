#ifndef GCOMPONENT_H
#define GCOMPONENT_H

#include "model/component.h"
#include "ui/objects/gcompositeobject.h"

#include <QObject>

/**
 * @brief Graphical object that wraps a `Component`.
 */
class GComponent : public GCompositeObject
{
    Q_OBJECT
public:
    // CONSTRUCTORS
    explicit GComponent(Component *obj);

    // GETTERS
    Component *get() override;
    const Component *get() const override;

private slots:
    void onContextEdit();

private:
    // EVENTS
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

#endif // GCOMPONENT_H
