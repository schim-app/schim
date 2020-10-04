#ifndef PAGE_H
#define PAGE_H

#include <QList>

#include "header.h"
#include "object.h"

class Sheet
{
private:
    float width = 297, height = 210;
    QList<Object*> objects;
    Header *header;
    QString title{"New sheet"};

public:
    Sheet();

    void addObject(Object *obj);

    QString getTitle() const;

    void setTitle(const QString &title);
};

#endif // PAGE_H
