/// @file global.h
#ifndef GLOBAL_H
#define GLOBAL_H

#include "symbols.h"

#include <QVariant>
#include <QColor>

// This header should not include other headers, because all other headers should
// in principle include this header
class ProjectManager;
class Database;

namespace Schim
{

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

ProjectManager *getProjectManager();
Database *getGlobalDatabase();

/**
 * Take a loose `path` and return a file path.
 * @param path A loose path, e.g. a component name, path relative to
 * installation directory, real path, etc.
 */
QString resolvePath(const QString &path);
QString resolveAbsPath(const QString &path);

/**
 * @brief Take an input in pixels and return a value that is independent of screen DPI.
 */
float dpiInvariant(float pxInput);

} // namespace Schim

using namespace Schim; // TODO temporary perhaps

#endif // GLOBAL_H
