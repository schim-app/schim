#ifndef XML_H
#define XML_H

#include "model/project.h"

#include <QtXml>
#include <exception>

/** Parse the XML file into a project */
Project *xmlParseProject(const QString &filename);
/** Parse a sheet from the specified XML element */
Sheet *xmlParseSheet(const QDomElement &element);

#endif // XML_H
