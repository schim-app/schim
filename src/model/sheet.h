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
    Header *header = new DefaultHeader;
    QString title{"New sheet"};

public:
    Sheet();
    ~Sheet();

    void addObject(Object *obj);

    float getWidth() const;
    float getHeight() const;
    QString getTitle() const;
    Header *getHeader();
    QList<Object*> &getObjects();

    void setTitle(const QString &title);
};

#endif // PAGE_H
