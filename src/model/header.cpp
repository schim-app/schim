#include "header.h"

#include "line.h"

Header::Header()
{

}

DefaultHeader::DefaultHeader()
    : Header()
{
    children = {
        new Rect(5, 5, 287, 195),
        new Line(5, 180, 292, 180),
    };
}
