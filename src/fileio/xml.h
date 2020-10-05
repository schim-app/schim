#ifndef XML_H
#define XML_H

#include "model/project.h"

#include <QtXml>
#include <exception>

Project *xmlParseProject(const QString &filename);
Sheet *xmlParseSheet(const QDomElement &element);

#endif // XML_H
