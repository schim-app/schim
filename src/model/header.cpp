#include "header.h"

Header::Header()
{

}

DefaultHeader::DefaultHeader()
    : Header()
{
    children.append(new Rect({0, 0}, 297, 210)); //TODO temp values
}
