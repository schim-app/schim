#include "xml.h"

#include "global.h"

#include <QFile>
#include <QMap>
#include <QXmlStreamReader>
#include <iostream>
#include <QDebug>

#define read_xml_file(filename) \
    QFile file(filename); \
    if (!file.open(QIODevice::ReadOnly)) \
        throw std::runtime_error("Unable to open XML file"); \
    QXmlStreamReader stream(&file)

void xmlTestRootTag(QXmlStreamReader &stream, const QString &tagname)
{
    while (!stream.atEnd())
    {
        stream.readNext();
        if (stream.isStartElement())
        {
            if (stream.name() != tagname)
                throw std::logic_error("Root tag is invalid");
            break;
        }
    }
}

Project *xmlParseProject(const QString &filename)
{
    read_xml_file(filename);
    xmlTestRootTag(stream, "project");

    Project *project = new Project;

    try
    {
        // Parse the root tag attributes
        for (const auto &attr : stream.attributes())
        {
            QString name = attr.name().toString();
            if (name == "title")
                project->setTitle(attr.value().toString());
        }
        // Parse contents
        while (!stream.atEnd())
        {
            stream.readNext();
            if (stream.isStartElement())
            {
                if (stream.name() == "sheet")
                    project->append(xmlParseSheet(stream));
                else
                    throw std::logic_error(std::string("Unrecognized tag: ")
                                           + stream.name().toString().toStdString());
            }
        }
    }
    catch (...)
    {
        delete project;
        throw;
    }

    return project;
}

Sheet *xmlParseSheet(QXmlStreamReader &stream)
{
    Sheet *sheet = new Sheet;

    try {
        // Parse the attributes
        for (const auto &attr : stream.attributes())
        {
            QString name = attr.name().toString();
            if (name == "title")
                sheet->setTitle(attr.value().toString());
        }

        while (!stream.atEnd() && !(stream.isEndElement() && stream.name() == "sheet"))
        {
            stream.readNext();
            if (stream.isStartElement())
            {
                if (stream.name() == "header")
                    sheet->setHeader(xmlParseHeader(stream));
                else
                    sheet->append(xmlParseObject(stream));
            }
        }

    }
    catch (...)
    {
        delete sheet; throw;
    }

    return sheet;
}

Object *xmlParseObject(QXmlStreamReader &stream)
{
    Object *object;
    if (stream.name() == "line")
        object = xmlParseLine(stream);
    else if (stream.name() == "rect")
        object = xmlParseRect(stream);
    else
        throw std::logic_error("Unknown object type");

    auto objectName = stream.name();

    // Eat up all remaining tokens related to this object
    while (!stream.atEnd() && !(stream.isEndElement() && stream.name() == objectName))
        stream.readNext();

    return object;
}

Line *xmlParseLine(QXmlStreamReader &stream)
{
    float x1 = 0, x2 = 0, y1 = 0, y2 = 0, linewidth = 0.5;

    for (const auto &attr : stream.attributes())
    {
        bool conversion_ok;
        if (attr.name() == "x1")
            x1 = attr.value().toString().toFloat(&conversion_ok);
        else if (attr.name() == "y1")
            y1 = attr.value().toString().toFloat(&conversion_ok);
        else if (attr.name() == "x2")
            x2 = attr.value().toString().toFloat(&conversion_ok);
        else if (attr.name() == "y2")
            y2 = attr.value().toString().toFloat(&conversion_ok);
        else if (attr.name() == "width")
            linewidth = attr.value().toString().toFloat(&conversion_ok);
        else
            throw std::logic_error("Unknown line attribute");
        if (!conversion_ok)
            throw std::logic_error("Line attributes are of invalid format");
    }
    Line *line = new Line(x1, y1, x2, y2);
    line->setLinewidth(linewidth);
    return line;
}

Rect *xmlParseRect(QXmlStreamReader &stream)
{
    float x = 0, y = 0, w = 50, h = 50;

    for (const auto &attr : stream.attributes())
    {
        bool conversion_ok;
        if (attr.name() == "x")
            x = attr.value().toString().toFloat(&conversion_ok);
        else if (attr.name() == "y")
            y = attr.value().toString().toFloat(&conversion_ok);
        else if (attr.name() == "w")
            w = attr.value().toString().toFloat(&conversion_ok);
        else if (attr.name() == "h")
            h = attr.value().toString().toFloat(&conversion_ok);
        else
            throw std::logic_error("Unknown rectangle attribute");
        if (!conversion_ok)
            throw std::logic_error("Rectangle attributes are of invalid format");
    }
    return new Rect(x, y, w, h);
}

CompositeObject *xmlParseCompositeObject(QXmlStreamReader &stream)
{
    //TODO implement
}

Header *xmlParseHeader(const QString &filename)
{
    read_xml_file(filename);

    xmlTestRootTag(stream, "header");

    return xmlParseHeader(stream);
}

Header *xmlParseHeader(QXmlStreamReader &stream)
{
    Header *header = nullptr;
    for (auto attr : stream.attributes())
    {
        if (attr.name() == "from")
        { // The header content is taken from another file
            if (header == nullptr)
                header = xmlParseHeader(attr.value().toString());
        }
    }

    if (header == nullptr) // The content was not taken from a file
        header = new Header;
    try
    {
        while (!stream.atEnd() && !(stream.isEndElement() && stream.name() == "header"))
        {
            stream.readNext();
            if (stream.isStartElement())
                header->append(xmlParseObject(stream));
        }
    }
    catch (...)
    {
        delete header; throw;
    }

    return header;
}
