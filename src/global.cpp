#include "global.h"

#include <QSettings>
#include <QFileInfo>
#include <QColor>

/**
 * Dynamically allocate a QSettings object and return a pointer to it.
 * This function automatically determines if the key is volatile and
 * returns the appropriate settings file.
 */
QSettings *getSettings(const QString &key)
{
    if (key == "default_path")
        return new QSettings("schim", "volatile");
    else
        return new QSettings("schim", "schim");
}

void changeSetting(const QString &key, const QString &value, bool sync)
{
    auto *settings = getSettings(key);

    settings->setValue(key, value);

    if (sync)
        settings->sync();

    delete settings;
}

QVariant getSetting(const QString &key, const QString &defaultValue)
{
    auto *settings = getSettings(key);

    auto value = settings->value(key, defaultValue);

    delete settings;
    return value;
}

QString findObject(const QString &name)
{
    //TODO implement
}

QString resolvePath(const QString &path)
{
    if (QFile(path).exists()) // The supplied path is either absolute or relative to the installation folder
        return path;
    else if (QFile(userSymbolPath + "/" + path).exists()) // The path is relative to the directory where user symbols are stored
        return userSymbolPath + "/" + path;
    else if (QFile(currentProjectPath + "/" + path).exists()) // The path is relative to the current project
        return currentProjectPath + "/" + path;
    else return path;
}
