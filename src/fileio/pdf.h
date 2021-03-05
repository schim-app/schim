#ifndef PDF_H
#define PDF_H

#include <QString>

#include "model/project.h"

/// @brief Write `project` to the file `filename`.
void pdfWriteProject(const Project *project, const QString &filename);

#endif // PDF_H
