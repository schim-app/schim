#ifndef SCHEMATICVIEW_H
#define SCHEMATICVIEW_H

#include <QGraphicsView>

#include <model/sheet.h>

#include "sheetscene.h"

class SheetView : public QGraphicsView
{
private:

public:
    /** Initialize a new SheetView with a blank Sheet of a default configuration. */
    SheetView();
    /** Initialize a new SheetView tied to the specified sheet */
    SheetView(Sheet *sheet);

    ~SheetView();

    SheetScene *scene();

private:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // SCHEMATICVIEW_H
