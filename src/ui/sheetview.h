/// @file sheetview.h
#ifndef SHEETVIEW_H
#define SHEETVIEW_H

#include "model/sheet.h"
#include "sheetscene.h"
#include "ui/widgets/insertcompleter.h"

#include <QGraphicsView>

class MainWindow;

/**
 * @brief A specialized `QGraphicsView` that displays a sheet of paper.
 */
class SheetView : public QGraphicsView
{
    Q_OBJECT
public:
    // CONSTRUCTORS
    /**
     * @brief This constructor is only used so that the MainWindow ui can be
     * generated properly.
     *
     * It should not be called elsewhere.
     */
    SheetView() = default;
    /**
     * @brief Initialize a new SheetView tied to the specified sheet.
     */
    explicit SheetView(Sheet *sheet, QWidget *parent = nullptr);
    ~SheetView();

    // GETTERS
    SheetScene *scene();

    // USER ACTIONS
    void setZoom(float zoom);
    void resetZoom();
    void zoomIn(float step = 1.2);
    void zoomOut(float step = 1.2);
    void scrollUp(Vim::N n = 0);
    void scrollDown(Vim::N n = 0);
    void scrollLeft(Vim::N n = 0);
    void scrollRight(Vim::N n = 0);
    void showContextMenu();
public slots:
    void insertPopup();

    // EVENTS
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void enterEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    // SLOTS
    void onCursorMoved();
    /**
     * @brief Called when the user selects a component to be inserted into the
     * scene.
     * @param obj The object that is to be inserted.
     */
    void onInsertionRequested(const Object &obj);

private:
    // OVERRIDDE QGraphicsView
    void drawForeground(QPainter *painter, const QRectF &rect) override;

    // HELPERS
    void init();
    void recalculateBaseZoom();
    void processRubberBandDrag(QMouseEvent *event);
    float zoom() const;
    void updateBackground();
    void updateCursorGuides();
    bool processVimAction(const Vim::Action &action);

private:
    // ATTRIBUTES
    /// Used to compensate for different screen sizes
    float baselineZoom = 1, userZoom = 1;
    /// Dynamically allocated just before first use, and persists afterwards
    InsertCompleter *insertCompleter{};
    /// Helper attributes for event processing
    QPoint _panStartPos, _selectStartPos;
    /// Used with rubber band dragging
    bool _selectionTypeDetermined = false, _rubberBandDragging = false;
};

#endif // SHEETVIEW_H
