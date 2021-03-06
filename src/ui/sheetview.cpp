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
#include <QtMath>
#include <QGraphicsSceneEvent>
#include <QScrollBar>

#include <iostream>

// CONSTRUCTORS

SheetView::SheetView(Sheet *sheet, QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new SheetScene(sheet));
    init();

    recalculateBaseZoom();
    updateBackground();

    setWhatsThis("This is a sheet of paper where you draw your schematics.");
    setStatusTip("class SheetView in src/ui/sheetview.h");
}

SheetView::~SheetView()
{
    delete scene();
    delete insertCompleter;
}

SheetScene *SheetView::scene()
{
    return static_cast<SheetScene*>(QGraphicsView::scene());
}

// USER ACTIONS

void SheetView::setZoom(float zoom)
{
    userZoom = qMin(qMax(zoom, 0.25f), 20.0f);
    resetTransform();
    scale(zoom * baselineZoom, zoom * baselineZoom);

    updateBackground();
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

void SheetView::scrollUp(Vim::N n)
{
    auto *bar = verticalScrollBar();
    bar->setValue(bar->value() - n * bar->singleStep());
    viewport()->update();
}

void SheetView::scrollDown(Vim::N n)
{
    auto *bar = verticalScrollBar();
    bar->setValue(bar->value() + n * bar->singleStep());
    viewport()->update();
}

void SheetView::scrollLeft(Vim::N n)
{
    auto *bar = horizontalScrollBar();
    bar->setValue(bar->value() - n * bar->singleStep());
    viewport()->update();
}

void SheetView::scrollRight(Vim::N n)
{
    auto *bar = horizontalScrollBar();
    bar->setValue(bar->value() + n * bar->singleStep());
    viewport()->update();
}

void SheetView::showContextMenu()
{
    QMenu menu(MainWindow::getInstance());

    // Create actions
    QAction insert("Insert...");
    QAction settings("Sheet settings...");
    QAction editHeader("Edit header... (TODO)");

    // Add actions to menu
    menu.addAction(&insert);
    menu.addAction(&settings);
    menu.addAction(&editHeader);

    //Connections
    connect(&insert, &QAction::triggered, this, &SheetView::insertPopup);
    connect(&settings, &QAction::triggered, this, []() {
        MainWindow::getInstance()->sheetSettings();
    });
    connect(&settings, &QAction::triggered, this, []() {
        // TODO
    });
    menu.exec(QCursor::pos());
}

void SheetView::insertPopup()
{
    if (!insertCompleter)
    {
        insertCompleter = new InsertCompleter(this);
        connect(insertCompleter, &InsertCompleter::insertionRequested,
                this, &SheetView::onInsertionRequested);
    }
    insertCompleter->move(QCursor::pos());
    insertCompleter->setFocus();
    insertCompleter->show();
}

// EVENTS

void SheetView::mousePressEvent(QMouseEvent *event)
{
    // Rubber band selection
    if (event->buttons() == Qt::LeftButton && !scene()->operation)
    {
        setDragMode(DragMode::RubberBandDrag);
        _selectStartPos = event->pos();
        _selectionTypeDetermined = false;
    }
    // Panning
    else if (event->buttons() == Qt::MidButton)
    {
        setDragMode(DragMode::ScrollHandDrag);
        _panStartPos = event->pos();
        return;
    }
    // Context menu
    else if (event->buttons() == Qt::RightButton)
    { // Show context menu
        // Had we used contextMenuEvent, then the context menu would be displayed
        // over a child's context menu
        event->ignore();
        QGraphicsView::mousePressEvent(event);
        if (event->isAccepted())
            return;
        else
            showContextMenu();
    }

    QGraphicsView::mousePressEvent(event);
}

void SheetView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MidButton)
    {
        // WARN doesn't work with some settings of ViewportAnchor
        const QPointF translation = mapToScene(event->pos()) - mapToScene(_panStartPos);
        translate(translation.x(), translation.y());
        _panStartPos = event->pos();
    }
    else if (event->buttons() == Qt::LeftButton)
        processRubberBandDrag(event);

    scene()->setSnapCursorGuides(false);

    QGraphicsView::mouseMoveEvent(event);

    //TODO move this to scene implementation??
    if (event->modifiers() == Qt::SHIFT)
        scene()->setSnapCursorGuides(true);
    else if (event->buttons() == Qt::MidButton)
        scene()->setSnapCursorGuides(false);

    // This is to update the cursor guides in the foreground
    // TODO maybe there is a way to only update the foreground?
    //viewport()->update();
    updateCursorGuides();
}

void SheetView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton || event->button() == Qt::MidButton)
    {
        // Disable rubber-band select and pan
        // This must come after base method call!
        setDragMode(DragMode::NoDrag);
        _rubberBandDragging = false;
        viewport()->setCursor(Qt::BlankCursor);
    }
}

void SheetView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    if (event->isAccepted()) return;

    Vim::registerKeyPress(event, [this](const Vim::Action &action) {
        return processVimAction(action);
    });
}

void SheetView::dropEvent(QDropEvent *event)
{
    setFocus();
    QGraphicsView::dropEvent(event);
}

void SheetView::leaveEvent(QEvent *event)
{
    scene()->showGuides(false);
    viewport()->update();
    QGraphicsView::leaveEvent(event);
    // TODO implement so that the guides are hidden when the mouse leaves the viewport, not the view
}

void SheetView::enterEvent(QEvent *event)
{
    // Update cursor guides to match actual cursor position
    QGraphicsSceneMouseEvent mouseEvent;
    mouseEvent.setScenePos(mapToScene(mapFromGlobal(cursor().pos())));
    scene()->mouseMoveEvent(&mouseEvent);

    scene()->showGuides(true);
    viewport()->update();
    QGraphicsView::enterEvent(event);
}

void SheetView::resizeEvent(QResizeEvent *event)
{
    recalculateBaseZoom();
    QGraphicsView::resizeEvent(event);
}

void SheetView::wheelEvent(QWheelEvent *event)
{
    // Zoom in/out
    if (QApplication::keyboardModifiers() & Qt::CTRL)
    {
        float delta = event->pixelDelta().isNull() ?
                    event->angleDelta().y() / 600. : event->pixelDelta().y() / 1000.;
        zoomIn(1 + delta);
    }
    else
        QGraphicsView::wheelEvent(event);
}

// SLOTS

void SheetView::onCursorMoved()
{
    auto pos = scene()->getCursorPos();

    // We do not want to trigger a mouseMoveEvent this time
    setMouseTracking(false);
    cursor().setPos(mapToGlobal(mapFromScene(pos)));
    qApp->processEvents();
    setMouseTracking(true);
}

void SheetView::onInsertionRequested(const Object &obj)
{
    setFocus();
    scene()->insertComponentOrHeader(obj);
}

// OVERRIDE QGraphicsView

void SheetView::drawForeground(QPainter *painter, const QRectF &rect)
{
    // A cosmetic pen with a width of 1
    QPen pen(qApp->palette().color(QPalette::Text), dpiInvariant(1));
    pen.setCosmetic(true);
    painter->setPen(pen);

    // Draw the grid
    if (scene()->gridEnabled)
    {
        //TODO hide this implementation detail in a function in global.h
        QRectF contentArea = scene()->sheet->getContentArea();
        QPointF center = contentArea.center();
        float Xmin = qMax(rect.left(), contentArea.left()),
                Xmax = qMin(rect.right(), contentArea.right()),
                Ymin = qMax(rect.top(), contentArea.top()),
                Ymax = qMin(rect.bottom(), contentArea.bottom());
        int Imax = (Xmax - center.x()) / scene()->gridX,
                Jmax = (Ymax - center.y()) / scene()->gridY;

        for (int i = qCeil((Xmin - center.x()) / scene()->gridX); i <= Imax; ++i)
            for (int j = qCeil((Ymin - center.y()) / scene()->gridY); j <= Jmax; ++j)
                painter->drawPoint(QPointF{center.x() + i * scene()->gridX, center.y() + j * scene()->gridY});
    }

    // Draw the cursor guides
    if (scene()->showCursorGuides)
    {
        QPointF pos = scene()->getCursorPos();

        QColor c = pen.color();
        c.setAlphaF(0.75);
        pen.setColor(c);
        painter->setPen(pen);

        // Horizontal
        painter->drawLine(QLineF{rect.left(), pos.y(), rect.right(), pos.y()});
        // Vertical
        painter->drawLine(QLineF{pos.x(), rect.top(), pos.x(), rect.bottom()});
    }
}

// HELPERS

void SheetView::init()
{
    setDragMode(DragMode::NoDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);
    setFocusPolicy(Qt::ClickFocus);
    setMouseTracking(true);
    setAcceptDrops(true);
    viewport()->setCursor(Qt::BlankCursor);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setRenderHint(QPainter::Antialiasing);
    // Needed to update the cursor guides when the viewport is scrolled
    // TODO Don't know if this is the optimal setting, but Minimal mode doesn't work properly
    // TODO try updating the scene in a small neighborhood of the guides
    setViewportUpdateMode(ViewportUpdateMode::FullViewportUpdate);
}

void SheetView::recalculateBaseZoom()
{
    if (scene() == nullptr)
        return;
    float widthRatio = parentWidget()->width() / scene()->sheet->getWidth();
    float heightRatio = parentWidget()->height() / scene()->sheet->getHeight();

    baselineZoom = 0.9 * qMin(widthRatio, heightRatio);
    setZoom(userZoom);
}

void SheetView::processRubberBandDrag(QMouseEvent *event)
{
    if (!_selectionTypeDetermined)
    { // Determine the rubber band type
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

void SheetView::updateCursorGuides()
{
    // For now just update the whole viewport
    return viewport()->update();
    // TODO enable this
    QPointF pos = mapFromScene(scene()->getCursorPos());

    QRectF rect(viewport()->rect());
    float t = rect.top(), b = rect.bottom(), l = rect.left(), r = rect.right();
    // Construct a thin rect around the vertical guide
    QRectF vert(pos.x() - 3, t, 6, rect.height());
    // Construct a thin rect around the horizontal guide
    QRectF horiz(l, pos.y() - 3, rect.width(), 6);
    // Update the viewport
    viewport()->update(vert.toRect());
    viewport()->update(horiz.toRect());
}

bool SheetView::processVimAction(const Vim::Action &action)
{
#define if_eq_do(name, act) if (action == name) { act(Vim::n()); }

    if_eq_do("zoom-in",				[this](Vim::N n){vimdo(n) zoomIn();})
    else if_eq_do("zoom-out",	 	[this](Vim::N n){vimdo(n) zoomOut();})
    else if_eq_do("zoom-set",		[this](Vim::N n) {
        if (n.raw() == 0) resetZoom(); else setZoom(n / 100.);
    })
    else if_eq_do("scroll-left",	scrollLeft)
    else if_eq_do("scroll-down",	scrollDown)
    else if_eq_do("scroll-up",		scrollUp)
    else if_eq_do("scroll-right",	scrollRight)
    else if (action == "insert")	insertPopup();
    else return false;
    return true;
}
