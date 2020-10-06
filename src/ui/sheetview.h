#ifndef SCHEMATICVIEW_H
#define SCHEMATICVIEW_H

#include <QGraphicsView>

#include <model/sheet.h>

#include "sheetscene.h"

class MainWindow;

class SheetView : public QGraphicsView
{

public:
    /** Initialize a new SheetView with a blank Sheet of a default configuration. */
    SheetView();
    /** Initialize a new SheetView tied to the specified sheet */
    SheetView(Sheet *sheet, QWidget *parent = nullptr);

    ~SheetView();

    void setZoom(float zoom);
    void resetZoom();
    void zoomIn(float step = 1.2);
    void zoomOut(float step = 1.2);

    SheetScene *scene();

private:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void init();
    void recalculateBaselineZoom();
    float zoom() const;
    void updateBackground();

    static int initTimerDisplayZoom();

private:

    // Used to compensate for different screen sizes
    float baselineZoom = 1, userZoom = 1;

    QPoint _panStartPos;
    static QTimer timerDisplayZoom;

};

#endif // SCHEMATICVIEW_H
