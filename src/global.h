#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVariant>
#include <QColor>

static QColor colorHover{0, 255, 0}, colorSelected{255, 0, 0};

#ifdef QT_DEBUG
static const char *systemSymbolPath = "../../symb", *systemMacroPath = "../../macros";
#else
static const char *systemSymbolPath = "symb", *systemMacroPath = "macros";
#endif

// TODO depends on OS
static QString currentProjectPath = "", userSymbolPath = "../../symb";

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

#endif // GLOBAL_H
