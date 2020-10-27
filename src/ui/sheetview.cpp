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

SheetView::SheetView(Sheet *sheet, QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new SheetScene(sheet));
    init();

    viewport()->setCursor(Qt::BlankCursor);

    recalculateBaselineZoom();
    updateBackground();

    connect(this, &SheetView::rubberBandChanged, this, &SheetView::onRubberBandChanged);

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
    if (event->buttons() == Qt::LeftButton && !scene()->operation)
    {
        setDragMode(DragMode::RubberBandDrag);
        _selectStartPos = event->pos();
        _selectionTypeDetermined = false;
    }
    else if (event->buttons() == Qt::MidButton)
    {
        setDragMode(DragMode::ScrollHandDrag);
        _panStartPos = event->pos();

        scene()->showGuides(false);
    }

    QGraphicsView::mousePressEvent(event);
}

void SheetView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::MidButton)
    {
        // WARN doesn't work with some settings of ViewportAnchor
        QPointF translation = mapToScene(event->pos()) - mapToScene(_panStartPos);
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
    viewport()->update();
}

void SheetView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::MidButton)
    {
        // Disable rubber-band select and pan
        setDragMode(DragMode::NoDrag);
        _rubberBandDragging = false;
        viewport()->setCursor(Qt::BlankCursor);

        scene()->showGuides(true);
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void SheetView::onRubberBandChanged(QRect rect, QPointF, QPointF)
{
    _rubberBandDragging = true;
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
    scene()->showGuides(true);
    viewport()->update();
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

void SheetView::drawForeground(QPainter *painter, const QRectF &rect)
{
    QPen pen(Qt::black, dpiInvariant(1));
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

        if (scene()->getSnapCursorGuides())
            pos = scene()->snap(pos);

        pen.setColor({64, 64, 64});
        painter->setPen(pen);

        // Horizontal
        painter->drawLine(QLineF{rect.left(), pos.y(), rect.right(), pos.y()});
        // Vertical
        painter->drawLine(QLineF{pos.x(), rect.top(), pos.x(), rect.bottom()});
    }

}

void SheetView::init()
{
    setDragMode(DragMode::NoDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);
    setMouseTracking(true);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setRenderHint(QPainter::Antialiasing);
    // Needed to update the cursor guides when the viewport is scrolled
    // TODO Don't know if this is the optimal setting, but Minimal mode doesn't work properly
    setViewportUpdateMode(ViewportUpdateMode::FullViewportUpdate);
    setAcceptDrops(true);
}

void SheetView::recalculateBaselineZoom()
{
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
