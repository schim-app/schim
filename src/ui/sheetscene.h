#ifndef SCHEMATICSCENE_H
#define SCHEMATICSCENE_H

#include "model/sheet.h"

#include <QGraphicsScene>
#include <QGraphicsItem>

class SheetView;

class SheetScene : public QGraphicsScene
{
    Sheet *sheet;

public:
    /**
     * Create a scene based on the specified sheet.
     * This should be considered the default constructor,
     * i.e. the one that contains the common functionality.
     */
    SheetScene(Sheet *sheet);
    SheetScene();

    Sheet *getSheet();
    void setSheet(Sheet *sheet);

private:

    void sheetChanged();
    void updatePageBackground(float zoomLevel);

    friend class SheetView;

private:

    QGraphicsRectItem *pageBackgroundItem;
};

#endif // SCHEMATICSCENE_H
