#ifndef SHEETSCENE_H
#define SHEETSCENE_H

#include "model/sheet.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QUndoStack>

class SheetView;
class QMouseEvent;
class GObject;
class Operation;

class SheetScene : public QGraphicsScene
{
    Sheet *sheet;
    QUndoStack undoStack{this};

public:
    /**
     * @brief Create a scene based on the specified sheet.
     *
     * This should be considered the default constructor,
     * i.e. the one that contains the common functionality.
     */
    SheetScene(Sheet *sheet);
    SheetScene();

    Sheet *getSheet();
    const Sheet *getSheet() const;
    QPointF getCursorPos() const;
    QPointF getSnappedCursorPos() const;
    bool getSnapCursorGuides() const;

    static bool isGridEnabled();
    static bool isSnapEnabled();
    static QSizeF getGridSize();

    void setSheet(Sheet *sheet);
    void setSnapCursorGuides(bool snap);
    static void setGridSize(float x, float y = -1);
    static void setGridSize(QSizeF size);

    void undo();
    void redo();
    void command(QUndoCommand *command);

    void cursorLeft();
    void cursorDown();
    void cursorUp();
    void cursorRight();

    void startOperation(Operation *op);
    /**
     * @brief Return the point on the grid that is closest to `pt`.
     *
     * Both the argument and return value are in scene coordinates.
     * @note If snap is disabled, the parameter is returned unchanged.
     */
    QPointF snap(const QPointF &pt) const;
    /**
     * @brief Return the point on the grid that is closest to `pt`.
     *
     * Both the argument and return value are in scene coordinates.
     * @note In contrast to `snap`, this method returns the snapped position
     * regardless of the enabled state of the snap feature.
     */
    QPointF forcedSnap(const QPointF &pt) const;
    void showGuides(bool show);

    void operationFinished(bool success = true);

private:

    void initGuides();
    QPointF constrainToContentArea(QPointF pt) const;

    // OVERRIDDEN METHODS

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    friend class SheetView;

private:
    // ATTRIBUTES

    QGraphicsRectItem *pageBackgroundItem;

    // The scene operation that is currently active
    Operation *operation{};
    // Cursor position that takes snapping into mind
    QPointF cursorPos;
    // This attribute has no effect if snap is not enabled
    bool snapGuides = false, showCursorGuides = true;

    // In millimeters
    static float gridX, gridY;
    static bool gridEnabled, snapEnabled; // only for test
};

#endif // SHEETSCENE_H
