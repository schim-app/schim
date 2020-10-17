#ifndef XML_H
#define XML_H

#include "model/project.h"
#include "model/line.h"

#include <QMap>
#include <QFile>
#include <exception>

class QXmlStreamReader;

/** Parse the XML file into a project */
Project *xmlParseProject(const QString &filename);
/** Parse a sheet from the specified XML stream */
Sheet *xmlParseSheet(QXmlStreamReader &stream);
/** Parse an object whose type is as yet undetermined */
Object *xmlParseObject(QXmlStreamReader &stream);
/** Parse a line object */
Line *xmlParseLine(QXmlStreamReader &stream);
/** Parse a rect object */
Rect *xmlParseRect(QXmlStreamReader &stream);
/** Parse a custom object */
CompositeObject *xmlParseCompositeObject(QXmlStreamReader &stream);

/** Parse a sheet header */
Header *xmlParseHeader(const QString &filename);
/** Parse a sheet header */
Header *xmlParseHeader(QXmlStreamReader &stream);

#endif // XML_H
