#include "global.h"

#include "ui/projectmanager.h"
#include "fileio/database.h"

#include <QSettings>
#include <QFileInfo>
#include <QColor>
#include <QApplication>
#include <QScreen>
#include <memory>

namespace Schim
{

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
    static std::unique_ptr<QSettings> normal, volat;
    if (
            key == "defaultPath" ||
            key == "printPath" ||
            key == "screenshotPath" ||
            key == ("GUI/geometry") ||
            key == ("GUI/windowState")
            )
    {
        if (!volat)
            volat = std::unique_ptr<QSettings>(new QSettings("schim", "volatile"));
        return volat.get();
    }
    else
    {
        if (!normal)
            normal = std::unique_ptr<QSettings>(new QSettings("schim", "schim"));
        return normal.get();
    }
}

void changeSetting(const QString &key, const QVariant &value, bool sync)
{
    auto *settings = getSettings(key);

    settings->setValue(key, value);

    if (sync)
        settings->sync();
}

QVariant getSetting(const QString &key, const QVariant &defaultValue)
{
    return getSettings(key)->value(key, defaultValue);
}

ProjectManager *getProjectManager()
{
    static std::unique_ptr<ProjectManager> pm;
    if (!pm)
        pm = std::unique_ptr<ProjectManager>(new ProjectManager);
    return pm.get();
}

Database *getGlobalDatabase()
{
    static std::unique_ptr<Database> globalDatabase;
    if (!globalDatabase)
        globalDatabase = std::unique_ptr<Database>(new Database(SYMBOL_DIR));
    return globalDatabase.get();
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
    if_exists_return(SYMBOL_DIR + "/" + path);
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

} // namespace Schim
