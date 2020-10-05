#include "sheetscene.h"

SheetScene::SheetScene(Sheet *sheet)
    : sheet(sheet)
{
    setBackgroundBrush(QBrush(Qt::Dense4Pattern));
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
