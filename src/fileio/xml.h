#ifndef XML_H
#define XML_H

#include "model/project.h"
#include "model/line.h"
#include "model/text.h"
#include "model/component.h"
#include "model/objectarray.h"
#include "model/special/linearobjectarray.h"

#include <QMap>
#include <QFile>
#include <exception>

class QXmlStreamReader;
class QXmlStreamWriter;

// PARSE BY FILENAME
Project *xmlParseProject(const QString &filename);
Object *xmlParseObject(const QString &filename);
CompositeObject *xmlParseCompositeObject(const QString &filename);
CompositeObject *xmlParseFromDxf(const QString &filename);
Header *xmlParseHeader(const QString &filename);
/**
 * @brief Get the index name of the element that is defined by the specified XML
 * file.
 *
 * The name is used to create an index of all symbols that are defined.
 */
QString xmlPeekName(const QString &filename);

/// Parse the XML file into a project
void xmlWriteProject(Project *project, const QString &filename);

/// Parse a sheet from the specified XML stream
Sheet *xmlParseSheet(QXmlStreamReader &stream);
void xmlWriteSheet(Sheet *sheet, QXmlStreamWriter &stream);

/// Parse an object whose type is as yet undetermined
Object *xmlParseObject(QXmlStreamReader &stream);
void xmlWriteObject(Object *obj, QXmlStreamWriter &stream);

/// Parse a line object
Line *xmlParseLine(QXmlStreamReader &stream);
void xmlWriteLine(Line *line, QXmlStreamWriter &stream);

/// Parse a rect object
Rect *xmlParseRect(QXmlStreamReader &stream);
void xmlWriteRect(Rect *rect, QXmlStreamWriter &stream);

/// Parse a text object
Text *xmlParseText(QXmlStreamReader &stream);
void xmlWriteText(Text *text, QXmlStreamWriter &stream);

/// Parse a terminal object
Terminal *xmlParseTerminal(QXmlStreamReader &stream);
void xmlWriteTerminal(Terminal *terminal, QXmlStreamWriter &stream);

/// Parse a custom object
CompositeObject *xmlParseCompositeObject(QXmlStreamReader &stream);
CompositeObject *xmlParseFromDxf(QXmlStreamReader &stream);

LinearObjectArray *xmlParseLinearObjectArray(QXmlStreamReader &stream);

/// Parse a sheet header
Header *xmlParseHeader(QXmlStreamReader &stream);
void xmlWriteHeader(Header *header, QXmlStreamWriter &stream);

Component *xmlParseComponent(QXmlStreamReader &stream);
void xmlWriteComponent(Component *component, QXmlStreamWriter &stream);

void xmlWriteVariable(const Variable &var, QXmlStreamWriter &stream);

#endif // XML_H
