#ifndef PDF_H
#define PDF_H

#include <QString>

#include "model/project.h"

void pdfWriteProject(const Project *project, const QString &filename);

#endif // PDF_H
