#ifndef MISCFILES_H
#define MISCFILES_H

#include <QString>

#include "model/object.h"
#include "model/header.h"

QString readMeta(const QString &filename);

CompositeObject *parseCompositeObject(const QString &filename);
Header *parseHeader(const QString &filename);

#endif // MISCFILES_H
