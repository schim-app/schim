#include "sheetview.h"

#include <QMouseEvent>

SheetView::SheetView()
{
    setScene(new SheetScene());
    setDragMode(DragMode::NoDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);
}

SheetView::SheetView(Sheet *sheet)
{
    setScene(new SheetScene(sheet));
    setDragMode(DragMode::NoDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);
    // TODO put common commands in the same place
}

SheetView::~SheetView()
{
    delete scene();
}

SheetScene *SheetView::scene()
{
    return (SheetScene*) QGraphicsView::scene();
}

void SheetView::mousePressEvent(QMouseEvent *event)
{
    // Object selection
    if (event->button() == Qt::LeftButton)
        setDragMode(DragMode::RubberBandDrag);
    else if (event->button() == Qt::MidButton)
        setDragMode(DragMode::ScrollHandDrag);

    QGraphicsView::mousePressEvent(event);
}

void SheetView::mouseReleaseEvent(QMouseEvent *event)
{
    // Object selection
    if (event->button() == Qt::LeftButton || event->button() == Qt::MidButton)
        setDragMode(DragMode::NoDrag);

    QGraphicsView::mouseReleaseEvent(event);
}
