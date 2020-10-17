#ifndef SHEET_H
#define SHEET_H

#include <QList>

#include "header.h"
#include "object.h"

/**
 * @brief This class models a sheet of paper.
 *
 * This class should always take care of the destruction
 * of the objects it contains. It inherits QList<Object*>
 * so that we don't have to reimplement convenience
 * functions that QList already provides.
 */
class Sheet : public QList<Object*>
{
private:
    float width = 297, height = 210;
    Header *header = nullptr;
    QString title{"New sheet"};

public:
    Sheet(bool defaultHeader = true);
    ~Sheet();

    float getWidth() const;
    float getHeight() const;
    QString getTitle() const;
    Header *getHeader();

    void setWidth(float width);
    void setHeight(float height);
    void setHeight() const;
    void setTitle(const QString &title);
    /**
     * @brief Set this sheet's header.
     *
     * The sheet becomes the owner of `header` and
     * is responsible for its deletion when the sheet's
     * destructor is called.
     */
    void setHeader(Header *header);
};

#endif // SHEET_H
