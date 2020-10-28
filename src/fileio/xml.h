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
Object *xmlParseObject(const QString &filename);
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
void xmlWriteText(Text *text, QXmlStreamWriter &stream);
//TODO write

/** Parse a custom object */
CompositeObject *xmlParseCompositeObject(const QString &filename);
CompositeObject *xmlParseCompositeObject(QXmlStreamReader &stream);
CompositeObject *xmlParseFromDxf(const QString &filename);
CompositeObject *xmlParseFromDxf(QXmlStreamReader &stream);

/** Parse a sheet header */
Header *xmlParseHeader(const QString &filename);
/** Parse a sheet header */
Header *xmlParseHeader(QXmlStreamReader &stream);
void xmlWriteHeader(Header *header, QXmlStreamWriter &stream);

CompositeObject *xmlParseComponent(QXmlStreamReader &stream);
/**
 * @brief Get the index name of the element that is defined by the specified XML file.
 *
 * The name is used to create an index of all symbols that are defined.
 */
QString xmlPeekName(const QString &filename);

#endif // XML_H
