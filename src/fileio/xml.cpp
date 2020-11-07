#include "xml.h"

#include "global.h"

#include <QFile>
#include <QMap>
#include <QXmlStreamReader>
#include <iostream>
#include <QDebug>

#include "model/component.h"

#define read_xml_file(filename) \
    QFile file(filename); \
    if (!file.open(QIODevice::ReadOnly)) \
        throw std::runtime_error(filename.toStdString() + ": Unable to open XML file for reading"); \
    QXmlStreamReader stream(&file)

#define write_xml_file(filename)

// PROTOTYPES
CompositeObject *parseCompositeObject(const QString &);
CompositeObject *dxfParseCompositeObject(const std::string &);

/********************
 * Helper functions *
 ********************/

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

void xmlConsumeFirstElement(QXmlStreamReader &stream)
{
    // Find the start element tag and stop there
    while (!stream.atEnd())
    {
        stream.readNext();
        if (stream.isStartElement())
            break;
    }
}

Variable xmlParseVariable(QXmlStreamReader &stream)
{
    Variable var;
    for (auto attr : stream.attributes())
    {
        if (attr.name() == "name")
            var.name = attr.value().toString();
        else if (attr.name() == "desc")
            var.description = attr.value().toString();
        else if (attr.name() == "value")
            var.value = attr.value().toString();
    }
    stream.skipCurrentElement();
    return var;
}

///////////////////////////////////////////////////////////////////////////

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
        delete project; throw;
    }

    return project;
}

void xmlWriteProject(Project *project, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        throw std::runtime_error("Unable to open XML file for writing");
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.setAutoFormattingIndent(4); // consider changing this to tabs

    stream.writeStartElement("project");
    stream.writeAttribute("title", project->getTitle());
    for (auto *sheet : *project)
        xmlWriteSheet(sheet, stream);
    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

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

        // Parse children
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

void xmlWriteSheet(Sheet *sheet, QXmlStreamWriter &stream)
{
    stream.writeStartElement("sheet");
    stream.writeAttribute("title", sheet->getTitle());

    stream.writeStartElement("header");
    stream.writeAttribute("from", "headers/defaultheader.xsym");
    stream.writeEndElement();

    for (auto *obj : *sheet)
        xmlWriteObject(obj, stream);

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

Object *xmlParseObject(const QString &filename)
{
    read_xml_file(filename);

    xmlConsumeFirstElement(stream);

    return xmlParseObject(stream);
}

Object *xmlParseObject(QXmlStreamReader &stream)
{
    Object *object;

    if (stream.name() == "line")
        object = xmlParseLine(stream);
    else if (stream.name() == "rect")
        object = xmlParseRect(stream);
    else if (stream.name() == "text")
        object = xmlParseText(stream);
    else if (stream.name() == "header")
        object = xmlParseHeader(stream);
    else if (stream.name() == "component")
        object = xmlParseComponent(stream);
    else
        throw std::logic_error("Unknown object type");

    auto objectName = stream.name();

    // Eat up all remaining tokens related to this object
    //while (!stream.atEnd() && !(stream.isEndElement() && stream.name() == objectName))
        //stream.readNext();
    //TODO skipCurrentElement
    stream.skipCurrentElement();

    return object;
}

void xmlWriteObject(Object *obj, QXmlStreamWriter &stream)
{
    // This is chiefly a product of my laziness
    #define if_cast_write(obj, TYPE) if (dynamic_cast<TYPE*>(obj)) xmlWrite##TYPE((TYPE*) obj, stream)

    if_cast_write(obj, Line);
    if_cast_write(obj, Rect);
    if_cast_write(obj, Text);
    if_cast_write(obj, Component);
}

///////////////////////////////////////////////////////////////////////////

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

void xmlWriteLine(Line *line, QXmlStreamWriter &stream)
{
    Line def; // Object with default values
    stream.writeStartElement("line");

    stream.writeAttribute("x1", QString::number(line->p1().x()));
    stream.writeAttribute("y1", QString::number(line->p1().y()));
    stream.writeAttribute("x2", QString::number(line->p2().x()));
    stream.writeAttribute("y2", QString::number(line->p2().y()));
    if (line->getLinewidth() != def.getLinewidth())
        stream.writeAttribute("width", QString::number(line->getLinewidth()));

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

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

void xmlWriteRect(Rect *rect, QXmlStreamWriter &stream)
{
    Rect def; // Object with default values
    stream.writeStartElement("rect");

    stream.writeAttribute("x", QString::number(rect->left()));
    stream.writeAttribute("y", QString::number(rect->top()));
    stream.writeAttribute("w", QString::number(rect->width()));
    stream.writeAttribute("h", QString::number(rect->height()));

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

Text *xmlParseText(QXmlStreamReader &stream)
{
    Text *text = new Text();

    try
    {
        float x = text->getPos().x(), y = text->getPos().y();
        for (const auto &attr : stream.attributes())
        {
            bool conversion_ok = true;
            if (attr.name() == "text")
                text->setText(attr.value().toString());
            else if (attr.name() == "x")
                x = attr.value().toString().toFloat(&conversion_ok);
            else if (attr.name() == "y")
                y = attr.value().toString().toFloat(&conversion_ok);
            else
                throw std::logic_error("Unknown attributes for text object");
            if (!conversion_ok)
                throw std::logic_error("Text object attributes are of invalid format");
            text->setPos({x, y});
        }
    }
    catch (...)
    {
        delete text; throw;
    }

    return text;
}

void xmlWriteText(Text *text, QXmlStreamWriter &stream)
{
    Text def; // Object with default values
    stream.writeStartElement("text");

    if (text->getText() != def.getText())
        stream.writeAttribute("text", text->getText());
    stream.writeAttribute("x", QString::number(text->getPos().x()));
    stream.writeAttribute("y", QString::number(text->getPos().y()));

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

CompositeObject *xmlParseCompositeObject(const QString &filename)
{
    read_xml_file(filename);

    xmlConsumeFirstElement(stream);

    return xmlParseCompositeObject(stream);
}

CompositeObject *xmlParseCompositeObject(QXmlStreamReader &stream)
{
    CompositeObject *obj = nullptr;

    for (auto attr : stream.attributes())
    {
        if (attr.name() == "from")
        { // The content is taken from another file
            if (obj == nullptr)
                obj = parseCompositeObject(resolvePath(attr.value().toString()));
        }
    }

    if (obj == nullptr)
        obj = new CompositeObject;

    try
    {
        while (!stream.atEnd() && !(stream.isEndElement() && stream.name() == "component"))
        {
            stream.readNext();
            if (stream.isStartElement())
                obj->append(xmlParseObject(stream));
        }
    }
    catch (...)
    {
        delete obj; throw;
    }

    return obj;
}

CompositeObject *xmlParseFromDxf(const QString &filename)
{
    read_xml_file(filename);

    xmlConsumeFirstElement(stream);

    return xmlParseFromDxf(stream);
}

CompositeObject *xmlParseFromDxf(QXmlStreamReader &stream)
{
    // Read the content of the "dxf" tag which is in base64 format
    QString content = stream.readElementText();
    auto arr = QByteArray::fromBase64(content.toUtf8(), QByteArray::Base64Encoding);

    return dxfParseCompositeObject(arr.toStdString());
}

///////////////////////////////////////////////////////////////////////////

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
                header = new Header(parseCompositeObject(resolvePath(attr.value().toString())));
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

void xmlWriteHeader(Header *header, QXmlStreamWriter &stream)
{
    //TODO detect whether header is the default header and add a
    // single from="headers/defaultheader.xsym" attribute
    stream.writeStartElement("header");

    for (auto obj : *header)
        xmlWriteObject(obj, stream);

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

Component *xmlParseComponent(QXmlStreamReader &stream)
{
    //TODO change return type
    Component *obj = nullptr;
    for (auto attr : stream.attributes())
    {
        if (attr.name() == "from")
        { // The content is taken from another file
            if (obj == nullptr)
                obj = new Component(parseCompositeObject(resolvePath(attr.value().toString())));
        }
    }

    if (obj == nullptr) // The content was not taken from a file
        obj = new Component;
    try
    {
        while (!stream.atEnd() && !(stream.isEndElement() && stream.name() == "component"))
        {
            stream.readNext();
            if (stream.isStartElement())
            {
                if (stream.name() == "var")
                    obj->addVariable(xmlParseVariable(stream));
                else if (stream.name() == "dxf")
                {
                    auto *container = xmlParseFromDxf(stream);
                    obj->append(*container);
                    container->clear();
                    delete container;
                }
                else
                    obj->append(xmlParseObject(stream));
            }
        }
    }
    catch (...)
    {
        delete obj; throw;
    }

    return obj;
}

void xmlWriteComponent(Component *component, QXmlStreamWriter &stream)
{
    stream.writeStartElement("component");

    for (auto var : component->getVariables())
    {
        stream.writeStartElement("var");
        stream.writeAttribute("name", var.name);
        stream.writeAttribute("value", var.value);
        stream.writeAttribute("desc", var.description);
        stream.writeEndElement();
    }

    for (auto obj : *component)
        xmlWriteObject(obj, stream);

    stream.writeEndElement();
}
///////////////////////////////////////////////////////////////////////////

QString xmlPeekName(const QString &filename)
{
    read_xml_file(filename);
    while (!stream.atEnd())
    {
        stream.readNext();
        if (stream.isStartElement())
        {
            for (const auto &attr : stream.attributes())
                if (attr.name() == "name")
                    return attr.value().toString();
        }
    }
    return "";
    // TODO temporarily disabled exception
    throw std::logic_error(filename.toStdString() + ": No name is defined for this object");
}
