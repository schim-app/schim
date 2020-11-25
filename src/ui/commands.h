#ifndef COMMANDS_H
#define COMMANDS_H

#include <model/sheet.h>
#include "sheetscene.h"
#include "ui/objects/gobject.h"
#include "ui/objects/gcompositeobject.h"

#include <QUndoCommand>


class CmdInsertObject : public QUndoCommand
{
public:
    CmdInsertObject(GObject *obj, SheetScene *scene);
    ~CmdInsertObject();
    void undo() override;
    void redo() override;

private:
    GObject *obj;
    SheetScene *scene;
};

class CmdDeleteSelection : public QUndoCommand
{
public:
    CmdDeleteSelection(QList<QGraphicsItem *> list, SheetScene *scene);
    ~CmdDeleteSelection();
    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem *> list;
    QList<GCompositeObject *> parentList;
    SheetScene *scene;
};

#endif // COMMANDS_H
