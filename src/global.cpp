#include "global.h"

#include <QSettings>
#include <QFileInfo>

/**
 * Dynamically allocate a QSettings object and return a pointer to it.
 * This function automatically determines if the key is volatile and
 * returns the appropriate settings file.
 */
QSettings *getSettingsObject(const QString &key)
{
    if (key == "default_path")
        return new QSettings("schematic", "volatile");
    else
        return new QSettings("schematic", "schematic");
}

void changeSetting(const QString &key, const QString &value, bool sync)
{
    auto *settings = getSettingsObject(key);

    settings->setValue(key, value);

    if (sync)
        settings->sync();

    delete settings;
}

QVariant getSetting(const QString &key, const QString &defaultValue)
{
    auto *settings = getSettingsObject(key);

    auto value = settings->value(key, defaultValue);

    delete settings;
    return value;
}
