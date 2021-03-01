#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVariant>
#include <QColor>

#include "fileio/database.h"

static QColor colorHover{0, 255, 0}, colorSelected{255, 0, 0};
extern Database *globalDatabase;

#define SHARE_DIR "../share/schim"

#ifdef QT_DEBUG
#define QT_HELP "../../docs/_build/qthelp/Schim.qhc"
static QString systemSymbolPath = "../../res/symb", systemMacroPath = "../../macros";
#else
#define QT_HELP SHARE_DIR "/Schim.qhc"
static QString systemSymbolPath = "../share/schim/symb", systemMacroPath = "macros";
#endif

// TODO depends on OS
static QString currentProjectPath = "", userSymbolPath = "../../res/symb";

/**
 * Change the value of the setting with the specified key.
 * @param sync Write the change to the settings file
 */
void changeSetting(const QString &key, const QString &value, bool sync = true);
/**
 * Read the setting with the specified key.
 * @param defaultValue Fallback value if the setting is not found
 */
QVariant getSetting(const QString &key, const QString &defaultValue);

QString resolvePath(const QString &path);

/**
 * @brief Take an input in pixels and return a value that is independent of screen DPI.
 */
float dpiInvariant(float pxInput);

#endif // GLOBAL_H
