#ifndef GLOBAL_H
#define GLOBAL_H

#include <QVariant>

#include "ui/mainwindow.h"

static MainWindow *mainWindow;

void changeSetting(const QString &key, const QString &value, bool sync = true);
QVariant getSetting(const QString &key, const QString &defaultValue);

#endif // GLOBAL_H
