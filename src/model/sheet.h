#ifndef SHEET_H
#define SHEET_H

#include <QList>

#include "header.h"
#include "object.h"

/**
 * This class models a sheet of paper.
 */
class Sheet : public QList<Object*>
{
private:
    float width = 297, height = 210;
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

    void setTitle(const QString &title);
};

#endif // SHEET_H
