#ifndef SHEETVIEW_H
#define SHEETVIEW_H

#include "model/sheet.h"
#include "sheetscene.h"
#include "ui/widgets/insertcompleter.h"

#include <QGraphicsView>

class MainWindow;

class SheetView : public QGraphicsView
{

public:
    /**
     * @brief This constructor is only used so that the MainWindow ui can be generated properly.
     *
     * It is not to be called elsewhere.
     */
    SheetView() = default;
    /**
     * @brief Initialize a new SheetView tied to the specified sheet.
     *
     * This is the constructor that implements common functionality.
     */
    SheetView(Sheet *sheet, QWidget *parent = nullptr);

    ~SheetView();

    void setZoom(float zoom);
    void resetZoom();
    void zoomIn(float step = 1.2);
    void zoomOut(float step = 1.2);

    SheetScene *scene();

private:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onRubberBandChanged(QRect rect, QPointF, QPointF);
    void onCursorChanged();
    void onInsertionRequested(Object *obj);

private:

    void leaveEvent(QEvent *event) override;
    void enterEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void drawForeground(QPainter *painter, const QRectF &rect) override;

    void init();
    void recalculateBaselineZoom();
    void processRubberBandDrag(QMouseEvent *event);
    float zoom() const;
    void updateBackground();
    void insertTriggered();

private:

    // Used to compensate for different screen sizes
    float baselineZoom = 1, userZoom = 1;
    /// Dynamically allocated just before first use, and persists afterwards
    InsertCompleter *insertCompleter{};

    QPoint _panStartPos, _selectStartPos;
    bool _selectionTypeDetermined = false, _rubberBandDragging = false;
};

#endif // SHEETVIEW_H
