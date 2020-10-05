#include "xml.h"

#include <QFile>
#include <QMap>

Project *xmlParseProject(const QString &filename)
{
    // Setup objects
    QDomDocument doc;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("Unable to read file");

    // Try to read the content of the xml file
    QString errorMsg;
    int errorLine, errorColumn;
    if (!doc.setContent(&file, false, &errorMsg, &errorLine, &errorColumn))
        throw std::logic_error(errorMsg.toStdString() + " at line "
                               + std::to_string(errorLine) + " column "
                               + std::to_string(errorColumn));
    file.close();

    // Create the root (project) element
    QDomElement root = doc.documentElement();

    if (root.tagName() != "project")
        throw std::logic_error("The file is not a project");

    // This is the object we will build up and return
    Project *project = new Project;

    // Parse all children of the root element
    for (auto child = root.firstChildElement(); !child.isNull(); child = child.nextSiblingElement())
    {
        if (child.tagName() == "sheet")
            project->append(xmlParseSheet(child));
        else
            throw std::logic_error("Unknown tag: line " + std::to_string(child.lineNumber())
                                   + ", column " + std::to_string(child.columnNumber()));
    }

    return project;
}

// Map of (attribute, default value) for sheets
QMap<QString, QString> sheetAttr {
    {"title", "New sheet"}
};

Sheet *xmlParseSheet(const QDomElement &element)
{
    Sheet *sheet = new Sheet;
    sheet->setTitle(element.attribute("title", sheetAttr["title"]));

    return sheet;
}
