#ifndef SHEETSCENE_H
#define SHEETSCENE_H

#include "model/sheet.h"

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QUndoStack>

class SheetView;
class QMouseEvent;

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

    /** @brief Start inserting a line into the sheet. */
    void insertLine();
    void undo();
    void redo();

private:

    void updatePageBackground(float zoomLevel);

    void keyPressEvent(QKeyEvent *event) override;

    void processInsertLine(QMouseEvent *event);

    friend class SheetView;

private:

    QGraphicsRectItem *pageBackgroundItem;
    bool _insertingLine;
};

#endif // SHEETSCENE_H
