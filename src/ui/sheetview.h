#ifndef SCHEMATICVIEW_H
#define SCHEMATICVIEW_H

#include <QGraphicsView>

#include <model/sheet.h>

#include "sheetscene.h"

class SheetView : public QGraphicsView
{
private:

public:
    /** Initialize a new SheetView with a blank Sheet with a default configuration. */
    SheetView();

    SheetScene *scene();

private:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // SCHEMATICVIEW_H
