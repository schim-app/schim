#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVariant>

#include "ui/mainwindow.h"

static MainWindow *mainWindow;
static QColor colorHover{0, 255, 0}, colorSelected{255, 0, 0};

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

#endif // GLOBAL_H
