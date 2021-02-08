#ifndef COMMANDS_H
#define COMMANDS_H

#include <model/sheet.h>
#include "sheetscene.h"
#include "ui/objects/gobject.h"
#include "ui/objects/gcompositeobject.h"
#include "ui/objects/gheader.h"

#include <QUndoCommand>


class CmdInsertObject : public QUndoCommand
{
public:
    CmdInsertObject(GObject *obj, SheetScene *scene, GCompositeObject *parent = nullptr);
    ~CmdInsertObject();
    void undo() override;
    void redo() override;

private:
    GObject *obj;
    GCompositeObject *parent;
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

class CmdChangeHeader : public QUndoCommand
{
public:
    CmdChangeHeader(Header *newHeader, SheetScene *scene);
    ~CmdChangeHeader();
    void undo() override;
    void redo() override;

private:
    Header *newHeader, *oldHeader;
    SheetScene *scene;
};

#endif // COMMANDS_H
