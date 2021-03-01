#ifndef SHEETSCENE_H
#define SHEETSCENE_H

#include "model/sheet.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QUndoStack>

class SheetView;
class QMouseEvent;
class GObject;
class GHeader;
class Operation;

class SheetScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /**
     * @brief Create a scene based on the specified sheet.
     *
     * This should be considered the default constructor,
     * i.e. the one that contains the common functionality.
     */
    SheetScene(Sheet *sheet);
    SheetScene();

    // GETTERS
    Sheet *getSheet();
    const Sheet *getSheet() const;

    QPointF getCursorPos() const;
    QPointF getSnappedCursorPos() const;
    bool getSnapCursorGuides() const;

    bool isGridEnabled();
    bool isSnapEnabled();
    QSizeF getGridSize();

    // SETTERS
    void setSheet(Sheet *sheet);
    /**
     * @brief Set the header in the scene and in the model.
     * @param hdr The new header.
     * @param destroy Whether to destroy the old header in the model.
     */
    void setHeader(Header *hdr, bool destroy = true);
    void setSnapCursorGuides(bool snap);

    void setGridSize(float x, float y = -1);
    void setGridSize(QSizeF size);
    void setGridEnabled(bool enabled);
    void setSnapToGrid(bool snap);

    // USER ACTIONS
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
    bool askHeaderChangeConfirmation() const;
    /**
     * @brief Try to set the sheet header to `hdr` and update the scene.
     *
     * @param hdr The new header.
     * @param changed [Out] Whether the header has been changed.
     * @param confirmed [Out] Whether the user confirmed the change.
     *
     * @note The user is prompted for confirmation only if the sheet
     * previously had a local header. If that is not the case, `confirmed`
     * is true by default.
     */
    void tryChangeHeader(Header *hdr,bool *changed = nullptr, bool *confirmed = nullptr);

    // OVERRIDEN METHODS
    GObject *itemAt(const QPointF &pt, const QTransform &deviceTransform);

    // OTHER METHODS
    void reload();

signals:
    void cursorChanged();

private:

    // HELPER FUNCTIONS
    QPointF constrainToContentArea(QPointF pt) const;
    void applyCursorMovement(const QPointF &pt);
    void insertComponentOrHeader(Object *obj);

    // OVERRIDDEN METHODS
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

private slots:
    // SLOTS
    void onOperationFinished();

    friend class SheetView;

private:
    // ATTRIBUTES

    Sheet *sheet;
    QUndoStack undoStack{this};
    GHeader *headerItem{};

    // A white sheet of paper
    QGraphicsRectItem *pageBackgroundItem;

    // The scene operation that is currently active
    Operation *operation{};
    // Cursor position that takes snapping into mind
    QPointF cursorPos;
    // This attribute has no effect if snap is not enabled
    bool snapGuides = false, showCursorGuides = true;
    GObject *hoveredItem{};

    // In millimeters
    float gridX = 5, gridY = 5;
    bool gridEnabled = true, snapEnabled = true;

    // FRIENDS
    friend class GObject;
};

#endif // SHEETSCENE_H
