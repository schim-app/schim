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

    void setSheet(Sheet *sheet);

    void undo();
    void redo();
    void command(QUndoCommand *command);

    void startOperation(Operation *op);
    void operationFinished(bool success = true);

private:

    void updatePageBackground(float zoomLevel);

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    friend class SheetView;

private:

    QGraphicsRectItem *pageBackgroundItem;
    Operation *operation{};
};

#endif // SHEETSCENE_H
