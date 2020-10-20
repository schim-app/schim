#ifndef XML_H
#define XML_H

#include "model/project.h"
#include "model/line.h"

#include <QMap>
#include <QFile>
#include <exception>

#include "model/text.h"

class QXmlStreamReader;
class QXmlStreamWriter;

/** Parse the XML file into a project */
Project *xmlParseProject(const QString &filename);
void xmlWriteProject(Project *project, const QString &filename);

/** Parse a sheet from the specified XML stream */
Sheet *xmlParseSheet(QXmlStreamReader &stream);
void xmlWriteSheet(Sheet *sheet, QXmlStreamWriter &stream);

/** Parse an object whose type is as yet undetermined */
Object *xmlParseObject(QXmlStreamReader &stream);
void xmlWriteObject(Object *obj, QXmlStreamWriter &stream);

/** Parse a line object */
Line *xmlParseLine(QXmlStreamReader &stream);
void xmlWriteLine(Line *line, QXmlStreamWriter &stream);

/** Parse a rect object */
Rect *xmlParseRect(QXmlStreamReader &stream);
void xmlWriteRect(Rect *rect, QXmlStreamWriter &stream);

/** Parse a text object */
Text *xmlParseText(QXmlStreamReader &stream);
//TODO write

/** Parse a custom object */
CompositeObject *xmlParseCompositeObject(QXmlStreamReader &stream);

/** Parse a sheet header */
Header *xmlParseHeader(const QString &filename);
/** Parse a sheet header */
Header *xmlParseHeader(QXmlStreamReader &stream);
void xmlWriteHeader(Header *header, QXmlStreamWriter &stream);

#endif // XML_H
