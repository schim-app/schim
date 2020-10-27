#include "miscfiles.h"

#include <QFile>
#include <QTextStream>

QString readMeta(const QString &filename)
{
    QFile file(filename + "/!meta");
    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Unable to open meta file.");

    return QTextStream(&file).readAll().trimmed();
}
