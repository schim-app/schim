#ifndef SCHEMATICSCENE_H
#define SCHEMATICSCENE_H

#include "model/sheet.h"

#include <QGraphicsScene>

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

    friend class SheetView;
};

#endif // SCHEMATICSCENE_H
