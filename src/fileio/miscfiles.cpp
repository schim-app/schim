#include "miscfiles.h"

#include "xml.h"
#include "dxf.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

QString readMeta(const QString &filename)
{
    QFile file(filename + "/!meta");
    if (!file.open(QIODevice::ReadOnly))
        throw std::runtime_error("Unable to open meta file.");

    return QTextStream(&file).readAll().trimmed();
}

CompositeObject *parseCompositeObject(const QString &filename)
{
    auto extension = filename.split(".").last();

    if (extension == "dxf")
        return dxfParseCompositeObject(filename);
    else if (extension == "xsym" || extension == "xml")
        return xmlParseCompositeObject(filename);
    else
        throw std::logic_error(QString(("Unrecognized file format: ") + extension).toStdString());
}

Header *parseHeader(const QString &filename)
{
    auto extension = filename.split(".").last();

    if (extension == "dxf")
        return nullptr; //TODO implement this
    else if (extension == "xsym" || extension == "xml")
        return xmlParseHeader(filename);
    else
        throw std::logic_error(QString(("Unrecognized file format: ") + extension).toStdString());
}
