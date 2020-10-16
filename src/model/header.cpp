#include "header.h"

#include "line.h"

Header::Header() { }

void Header::update()
{
    clear();
}

void Header::init()
{

}

void Header::clear()
{

}

/*****************
 * DefaultHeader *
 *****************/

DefaultHeader::DefaultHeader()
{
    init();
    update();
}

void DefaultHeader::update()
{
    clear();
    //TODO add configurable part
}

void DefaultHeader::init()
{
    append({
        new Rect(5, 5, 287, 195),
        new Line(5, 180, 292, 180),
    });
}

void DefaultHeader::clear()
{
    // The optional objects start from a certain index
    for (int i = 2; i < size(); ++i)
        delete (*this)[i];

    erase(begin() + 2, end());
}
