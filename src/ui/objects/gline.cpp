#include "gline.h"

#include "ui/sheetscene.h"

#include <QtMath>
#include <QPainter>
//TODO temp
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include "ui/mainwindow.h"


GLine::GLine(Line *obj)
    : GObject(obj)
{
    handles = {
        new GObjectHandle(this),
        new GObjectHandle(this),
    };
    reload();
}

Line *GLine::get()
{
    return (Line*) obj;
}

const Line *GLine::get() const
{
    return (Line*) obj;
}

QPainterPath GLine::shape() const
{
    // TODO do this right, too lazy to do it now
    auto rect = handles[0]->rect();
    float dx = rect.bottomRight().x(),
            dy = rect.bottomRight().y();

    QPolygonF poly;
    poly <<  -QPointF{dx, dy}
      << QPointF{-dx, dy}
      << get()->p2() - get()->p1() + QPointF{dx, dy}
      << get()->p2() - get()->p1() + QPointF{dx, -dy};

    QPainterPath path;
    path.addPolygon(poly);
    return path;
}

void GLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GObject::paint(painter, option, widget);
    auto pen = painter->pen();
    // We are dividing line width by 2 to compensate for antialiasing
    pen.setWidthF(get()->linewidth / 2);
    painter->setPen(pen);
    painter->drawLine(0, 0, get()->dx(), get()->dy());
    MainWindow::instance->dispValue(QString("%1  %2").arg((handles[1]->pos() - handles[0]->pos()).y()).arg(get()->dy()));
}

void GLine::reload()
{
    setPos(get()->p1());
    handles[0]->setPos(QPointF{0, 0});
    handles[1]->setPos({get()->dx(), get()->dy()});
}

void GLine::apply()
{
    get()->setP1(handles[0]->pos());
    get()->setP2(handles[1]->pos());
}

void GLine::handleChanged(GObjectHandle *handle)
{
    if (handle == handles[0])
        get()->setP1(mapToScene(handle->pos()));
    else if (handle == handles[1])
        get()->setP2(mapToScene(handle->pos()));
    reload();
    for (auto *handle : handles)
        handle->update();
    //TODO Is there any way I can do this without updating the whole scene?
    // Currently, if I don't do this there are graphical artifacts
    if (scene())
        scene()->update();
    if (handles.size() > 1)
        MainWindow::instance->dispValue(QString("%1  %2").arg(handles[1]->pos().y()).arg(get()->p2().y()));
}
