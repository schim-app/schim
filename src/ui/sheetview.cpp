#include "sheetview.h"

#include "mainwindow.h"
#include "global.h"

#include <QMouseEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QTransform>
#include <QTimer>
#include <QApplication>
#include <QWheelEvent>

QTimer SheetView::timerDisplayZoom{nullptr};

SheetView::SheetView()
{
    setScene(new SheetScene());
    init();
}

SheetView::SheetView(Sheet *sheet, QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new SheetScene(sheet));
    init();
    // TODO put common commands in the same place

    recalculateBaselineZoom();
    updateBackground();
}

SheetView::~SheetView()
{
    delete scene();
}

void SheetView::setZoom(float zoom)
{
    userZoom = qMin(qMax(zoom, 0.05f), 20.0f);
    resetTransform();
    scale(zoom * baselineZoom, zoom * baselineZoom);
    updateBackground();

    // Display the current zoom level
    //TODO
}

void SheetView::resetZoom()
{
    setZoom(1);
}

void SheetView::zoomIn(float step)
{
    setZoom(userZoom * step);
}

void SheetView::zoomOut(float step)
{
    setZoom(userZoom / step);
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
    {
        setDragMode(DragMode::ScrollHandDrag);
        _panStartPos = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}

void SheetView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MidButton)
    {
        QPointF translation = mapToScene(event->pos()) - mapToScene(_panStartPos);
        translate(translation.x(), translation.y());
        _panStartPos = event->pos();

        /* TODO maybe enable this version of the functionality
        QRectF sRect = sceneRect();
        sRect.translate(translation);
        setSceneRect(sRect);
        */
    }
    QGraphicsView::mouseMoveEvent(event);
}

void SheetView::mouseReleaseEvent(QMouseEvent *event)
{
    // Object selection
    if (event->button() == Qt::LeftButton || event->button() == Qt::MidButton)
        setDragMode(DragMode::NoDrag);

    QGraphicsView::mouseReleaseEvent(event);
}

void SheetView::resizeEvent(QResizeEvent *event)
{
    recalculateBaselineZoom();
    QGraphicsView::resizeEvent(event);
}

void SheetView::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers() & Qt::CTRL)
    {
        float delta = event->pixelDelta().isNull() ? event->angleDelta().y() / 600. : event->pixelDelta().y() / 1000.;
        zoomIn(1 + delta);
    }
    else
        QGraphicsView::wheelEvent(event);
}

void SheetView::init()
{
    setDragMode(DragMode::NoDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);
    setMouseTracking(true);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing);
}

void SheetView::recalculateBaselineZoom()
{
    float widthRatio = parentWidget()->width() / scene()->sheet->getWidth();
    float heightRatio = parentWidget()->height() / scene()->sheet->getHeight();

    baselineZoom = 0.9 * qMin(widthRatio, heightRatio);
    setZoom(userZoom);

    scene()->updatePageBackground(baselineZoom * userZoom);
}

float SheetView::zoom() const
{
    return userZoom * baselineZoom;
}

void SheetView::updateBackground()
{
    auto brush = QBrush(QColor(0, 0, 0, 48), Qt::Dense4Pattern);
    brush.setTransform(QTransform().scale(10 / zoom(), 10 / zoom()));
    setBackgroundBrush(brush);
}

int SheetView::initTimerDisplayZoom()
{
    timerDisplayZoom.setInterval(1500);
    timerDisplayZoom.setSingleShot(true);
    //TODO connect...
}
