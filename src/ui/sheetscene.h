/// @file sheetscene.h
#ifndef SHEETSCENE_H
#define SHEETSCENE_H

#include "model/sheet.h"
#include "model/component.h"
#include "model/terminal.h"

#include "ui/vim.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QUndoStack>
#include <QList>
#include <QPair>

class GObject;
class GHeader;
class GComponent;
class GTerminal;
class GConnectionSuggester;
class SceneOperation;

/**
 * @brief A specialized `QGraphicsScene` containing a sheet of paper.
 */
class SheetScene : public QGraphicsScene
{
    Q_OBJECT

public:
    // CONSTRUCTORS
    /**
     * @brief Construct a scene based on the specified sheet.
     * @throws std::logic_error If sheet is null.
     * @note The scene does not take ownership of `sheet`.
     */
    explicit SheetScene(Sheet *sheet);

    // GETTERS
    Sheet *getSheet();
    const Sheet *getSheet() const;
    /**
     * @brief Return the raw cursor position, before applying snap.
     */
    QPointF getRawCursorPos() const;
    /**
     * @brief Return the processed cursor pos.
     *
     * If snap is disabled the raw cursor position is returned. If snap is
     * enabled, the snapped cursor position is returned.
     * @see getCursorPos
     * @see getSnappedCursorPos
     */
    QPointF getCursorPos() const;
    QPointF getSnappedCursorPos() const;
    bool getSnapCursorGuides() const;
    bool isGridEnabled() const;
    bool isSnapEnabled() const;
    QSizeF getGridSize() const;
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
    void setSnapEnabled(bool snap);

    // USER ACTIONS
    void undo(Vim::N n = 0);
    void redo(Vim::N n = 0);
    void command(QUndoCommand *command);
    void cursorLeft(Vim::N n = 0);
    void cursorDown(Vim::N n = 0);
    void cursorUp(Vim::N n = 0);
    void cursorRight(Vim::N n = 0);
    void gridIncrease(Vim::N n = 0);
    void gridDecrease(Vim::N n = 0);
    /**
     * @brief Start a scene operation.
     *
     * The scene takes ownership of `op`, which must be dynamically allocated.
     * Any active operations will be canceled.
     */
    void startOperation(SceneOperation *op);
    void selectTexts();
    void selectPrimitive();
    void insertLine();
    void insertRect();
    void insertText();
    void suggestConnections(GComponent *component);
    void showGuides(bool show);
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
    void tryChangeHeader(Header *hdr, bool *changed = nullptr,
                         bool *confirmed = nullptr);

    // OVERRIDDE QGraphicsScene
    GObject *itemAt(const QPointF &pt, const QTransform &deviceTransform);

    // OTHER METHODS
    void reload();

signals:
    void itemMoveRequested();

private slots:
    void onSelectionChanged();

private:
    // HELPERS
    QPointF constrainToContentArea(QPointF pt) const;
    void applyCursorMovement(const QPointF &pt);
    /**
     * @brief Called when the user drags an item from the symbol browser into the
     * scene.
     */
    void insertComponentOrHeader(const Object &obj);
    bool askHeaderChangeConfirmation() const;
    QList<QPair<Terminal::Prong, Terminal::Prong>>
        getConnectionSuggestions(GComponent *component);
    void registerConnectionSuggestion(Terminal::Prong a, Terminal::Prong b);

    // EVENTS
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    bool processVimAction(const Vim::Action &action);

private slots:
    // SLOTS
    void onOperationFinished();

    friend class SheetView;

private:
    // ATTRIBUTES
    Sheet *sheet{};
    QUndoStack undoStack{this};
    GHeader *headerItem{};
    /// A white sheet of paper
    QGraphicsRectItem *pageBackgroundItem;
    QList<GConnectionSuggester*> _suggesters;
    /// The scene operation that is currently active
    SceneOperation *operation{};
    /// Cursor position with no snap applied
    QPointF rawCursorPos;
    /// This attribute has no effect if snap is not enabled
    bool snapGuides = false,
        showCursorGuides = true;
    GObject *hoveredItem{};
    /// In millimeters
    float gridX = 5, gridY = 5;
    bool gridEnabled = true, snapEnabled = true;

    // FRIENDS
    friend class GObject;
};

#endif // SHEETSCENE_H
