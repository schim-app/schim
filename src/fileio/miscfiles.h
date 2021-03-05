/**
 * @file miscfiles.h
 * 
 * This file contains functions that are useful for converting files, regardless
 * of the format being converted from/to.
 */
#ifndef MISCFILES_H
#define MISCFILES_H

#include <QString>

#include "model/object.h"
#include "model/header.h"

/**
 * @brief Read a !meta file containing information about symbols.
 * @param filename The file which should be read.
 * @return The contents of the file.
 *
 * A !meta file is a file that is placed in a directory containing symbols. This
 * file determines information about this directory.  Among other things, this
 * file is used to determine the display name of a category of symbols.
 */
QString readMeta(const QString &filename);

/**
 * @brief Parse a `CompositeObject` from file `filename`.
 *
 * The format of the file is automatically determined and the appropriate
 * specialized function is called.
 */
CompositeObject *parseCompositeObject(const QString &filename);
/**
 * @brief Parse `CompositeObject` from file `filename`.
 *
 * The format of the file is automatically determined and the appropriate
 * specialized function is called.
 */
Header *parseHeader(const QString &filename);

#endif // MISCFILES_H
