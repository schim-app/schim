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
#include <QMenu>
#include <QString>
#include <QCompleter>

SheetView::SheetView(Sheet *sheet, QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new SheetScene(sheet));
    init();

    // Initialize guides
    hGuide = scene()->addLine({});
    vGuide = scene()->addLine({});
    hGuide->setPos({});
    vGuide->setPos({});
    QPen pen(QColor{96, 96, 96}, 0);
    hGuide->setPen(pen);
    vGuide->setPen(pen);

    viewport()->setCursor(Qt::BlankCursor);

    updateGuides();
    recalculateBaselineZoom();
    updateBackground();


    // Create actions
    // TODO change this when vim becomes disable-able
    QAction *actionInsert = new QAction;
    actionInsert->setShortcut({Qt::Key_I});

    addAction(actionInsert);
    connect(actionInsert, &QAction::triggered, this, &SheetView::insertTriggered);
}

SheetView::~SheetView()
{
    delete scene();
}

void SheetView::setZoom(float zoom)
{
    userZoom = qMin(qMax(zoom, 0.25f), 20.0f);
    resetTransform();
    scale(zoom * baselineZoom, zoom * baselineZoom);
    updateBackground();
    updateGuides();
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
    {
        setDragMode(DragMode::RubberBandDrag);
        _selectStartPos = event->pos();
        _selectionTypeDetermined = false;
    }
    else if (event->button() == Qt::MidButton)
    {
        setDragMode(DragMode::ScrollHandDrag);
        _panStartPos = event->pos();
        vGuide->hide();
        hGuide->hide();
    }

    QGraphicsView::mousePressEvent(event);
}

void SheetView::mouseMoveEvent(QMouseEvent *event)
{
    updateGuides();
    if (event->buttons() == Qt::MidButton)
    {
        // TODO doesn't work -- fix it
        QPointF translation = mapToScene(event->pos()) - mapToScene(_panStartPos);
        translate(translation.x(), translation.y());
        _panStartPos = event->pos();
    }
    else if (event->buttons() == Qt::LeftButton && !_selectionTypeDetermined)
    {
        float dx = event->pos().x() - _selectStartPos.x();
        if (dx > 0)
        {
            setRubberBandSelectionMode(Qt::ContainsItemShape);
            setStyleSheet("selection-background-color: normal");
        }
        else if (dx < 0)
        {
            setRubberBandSelectionMode(Qt::IntersectsItemShape);
            setStyleSheet("selection-background-color: green");
        }

        if (dx > 4 || dx < -4)
            _selectionTypeDetermined = true;
    }
    else
        QGraphicsView::mouseMoveEvent(event);
}

void SheetView::mouseReleaseEvent(QMouseEvent *event)
{
    // Object selection
    if (event->button() == Qt::LeftButton || event->button() == Qt::MidButton)
    {
        setDragMode(DragMode::NoDrag);
        viewport()->setCursor(Qt::BlankCursor);
        vGuide->show();
        hGuide->show();
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void SheetView::leaveEvent(QEvent *event)
{
    vGuide->hide();
    hGuide->hide();
    QGraphicsView::leaveEvent(event);
}

void SheetView::enterEvent(QEvent *event)
{
    vGuide->show();
    hGuide->show();
    QGraphicsView::enterEvent(event);
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
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setRenderHint(QPainter::Antialiasing);
    // Needed to update the cursor guides when the viewport is scrolled
    setViewportUpdateMode(ViewportUpdateMode::BoundingRectViewportUpdate);
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

void SheetView::updateGuides()
{
    auto pos = mapToScene(mapFromGlobal(QCursor::pos()));

    QRectF rect = {mapToScene(0, 0), mapToScene(viewport()->width(), viewport()->height())};
    hGuide->setLine(rect.left(), pos.y(), rect.right(), pos.y());
    vGuide->setLine(pos.x(), rect.top(), pos.x(), rect.bottom());
}

void SheetView::insertTriggered()
{
    //TODO remove this
    QStringList wordList;
    wordList << "I" << "indigo" << "ind" << "india" << "am" << "boo boo";
    QLineEdit *lineEdit = new QLineEdit(this);

    QCompleter *completer = new QCompleter(wordList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    lineEdit->setCompleter(completer);

    lineEdit->move(QCursor::pos());
    lineEdit->setFocus();
    lineEdit->show();
}
