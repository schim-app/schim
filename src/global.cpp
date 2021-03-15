#include "global.h"

#include "ui/projectmanager.h"
#include "fileio/database.h"

#include <QSettings>
#include <QFileInfo>
#include <QColor>
#include <QApplication>
#include <QScreen>

// TODO perform deletion of dynamically allocated objects in this file!

/**
 * @brief Dynamically allocate a QSettings object and return a pointer to it.
 *
 * This function automatically determines if the key is volatile and
 * returns the appropriate settings file.
 *
 * @see For more information look at User Manual - Advanced topics -
 * Configuration files or the manpage schim-config.
 */
QSettings *getSettings(const QString &key)
{
    if (
            key == "defaultPath" ||
            key == "printPath" ||
            key == "screenshotPath" ||
            key == ("GUI/geometry") ||
            key == ("GUI/windowState")
            )
        return new QSettings("schim", "volatile");
    else
        return new QSettings("schim", "schim");
}

void changeSetting(const QString &key, const QVariant &value, bool sync)
{
    auto *settings = getSettings(key);

    settings->setValue(key, value);

    if (sync)
        settings->sync();

    delete settings;
}

QVariant getSetting(const QString &key, const QVariant &defaultValue)
{
    auto *settings = getSettings(key);

    auto value = settings->value(key, defaultValue);

    delete settings;
    return value;
}

ProjectManager *getProjectManager()
{
    static ProjectManager *pm = nullptr;
    if (pm == nullptr)
        return pm = new ProjectManager;
    else
        return pm;
}

Database *getGlobalDatabase()
{
    static Database *globalDatabase = nullptr;
    if (globalDatabase == nullptr)
        return globalDatabase = new Database(systemSymbolPath);
    else
        return globalDatabase;
}

QString resolvePath(const QString &path)
{
    // TODO this function needs to be heavily tested
#define if_exists_return(arg) if (QFile(arg).exists()) return arg

    // The supplied path is absolute
    if_exists_return(path);
    // The path is relative to the current project
    if_exists_return(currentProjectPath + "/" + path);
    // The path is relative to the directory where user symbols are stored
    if_exists_return(userSymbolPath + "/" + path);
    // The path is relative to the system-wide symbol path
    if_exists_return(QCoreApplication::applicationDirPath() + "/" + systemSymbolPath + "/" + path);
    // The path is relative to the executable dir
    if_exists_return(QCoreApplication::applicationDirPath() + "/" + path);
    // Else
    return path;

#undef if_exists_return
}

QString resolveAbsPath(const QString &path)
{
    return QFileInfo(resolvePath(path)).absoluteFilePath();
}

float dpiInvariant(float pxInput)
{
    return pxInput * QApplication::screens().at(0)->logicalDotsPerInch() / 141.21;
}
