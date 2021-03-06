#ifndef GLOBAL_H
#define GLOBAL_H

#include "symbols.h"

#include <QVariant>
#include <QColor>

#include "fileio/database.h"

/// The global database of components
extern Database *globalDatabase;

static QString currentProjectPath = "", userSymbolPath = "~/.local/share/schim/symb";

static QString systemSymbolPath = "../share/schim/symb", systemMacroPath = "macros";

/**
 * Change the value of the setting with the specified key.
 * @param sync Write the change to the settings file
 */
void changeSetting(const QString &key, const QVariant &value, bool sync = true);
/**
 * Read the setting with the specified key.
 * @param defaultValue Fallback value if the setting is not found
 */
QVariant getSetting(const QString &key, const QVariant &defaultValue);

/**
 * Take a loose `path` and return a file path.
 * @param path A loose path, e.g. a component name, path relative to
 * installation directory, real path, etc.
 */
QString resolvePath(const QString &path);

/**
 * @brief Take an input in pixels and return a value that is independent of screen DPI.
 */
float dpiInvariant(float pxInput);

#endif // GLOBAL_H
