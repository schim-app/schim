#include "sheetscene.h"

#include "objects/gobject.h"

#include <QColor>
#include "objects/gline.h"

SheetScene::SheetScene(Sheet *sheet)
    : sheet(sheet)
{
    if (sheet == nullptr) return;

    pageBackgroundItem = addRect({0, 0, sheet->getWidth(), sheet->getHeight()}, {}, QColorConstants::White);
    // Populate the scene with the sheet contents
    for (auto *obj : sheet->getObjects())
        addItem(GObject::assign(obj));
    addItem(GObject::assign(sheet->getHeader()));
}

SheetScene::SheetScene()
    : SheetScene(nullptr) { }

Sheet *SheetScene::getSheet()
{
    return sheet;
}

void SheetScene::setSheet(Sheet *sheet)
{
    this->sheet = sheet;
}

/*****************
 * Miscellaneous *
******************/

void SheetScene::sheetChanged()
{
    // TODO implement
}

void SheetScene::updatePageBackground(float zoomLevel)
{
    auto pen = pageBackgroundItem->pen();
    pen.setWidth(1 / zoomLevel);
    pageBackgroundItem->setPen(pen);
}
