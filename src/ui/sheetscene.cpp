#include "sheetscene.h"

SheetScene::SheetScene(Sheet *sheet)
    : sheet(sheet)
{
    setBackgroundBrush(QBrush(Qt::Dense4Pattern));
}

SheetScene::SheetScene()
    : SheetScene(new Sheet) { }

Sheet *SheetScene::getSheet()
{
    return sheet;
}
