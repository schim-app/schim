#include "xml.h"

#include "global.h"
#include "model/project.h"
#include "model/component.h"

#include <QFile>
#include <QMap>
#include <QXmlStreamReader>
#include <iostream>
#include <QDebug>
#include <QFileSystemWatcher>

#define read_xml_file(filename) \
    QFile file(filename); \
    if (!file.open(QIODevice::ReadOnly)) \
        throw std::runtime_error("Unable to open XML file for reading " + filename.toStdString() ); \
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
                throw std::logic_error("Root tag is invalid: " + stream.name().toString().toStdString());
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
    foreach (auto attr, stream.attributes())
    {
        if (attr.name() == "names")
            var.names = attr.value().toString().split(' ');
        else if (attr.name() == "desc")
            var.description = attr.value().toString();
        else if (attr.name() == "value")
            var.value = attr.value().toString();
    }
    stream.skipCurrentElement();
    return var;
}

void xmlReadProperties(Object *object, QXmlStreamReader &stream)
{
    try
    {
        foreach (const auto &attr, stream.attributes())
            object->setProperty(attr.name().toString(), attr.value().toString());
    }
    catch (std::exception &e) { delete object; throw; }
}

/*********************
 * Parse by filename *
 *********************/

Object *xmlParseObject(const QString &filename)
{
    // TODO What happens when the file is changed? Currently, this creates an
    // object using the last loaded file version as the template. In the future,
    // make it so that when a file is changed, the cache is automatically updated.
    static QMap<QString, Object *> cache;

    auto iter = cache.find(filename);
    if (iter != cache.end())
        return (*iter)->clone();

    read_xml_file(filename);
    xmlConsumeFirstElement(stream);

    Object *obj = xmlParseObject(stream);
    cache[filename] = obj->clone();

    return obj;
}

CompositeObject *xmlParseCompositeObject(const QString &filename)
{
    read_xml_file(filename);

    xmlConsumeFirstElement(stream);

    return xmlParseCompositeObject(stream);
}

CompositeObject *xmlParseFromDxf(const QString &filename)
{
    read_xml_file(filename);

    xmlConsumeFirstElement(stream);

    return xmlParseFromDxf(stream);
}

Header *xmlParseHeader(const QString &filename)
{
    static QMap<QString, Header *> cache;

    auto iter = cache.find(filename);
    if (iter != cache.end())
        return (*iter)->clone();

    read_xml_file(filename);
    xmlTestRootTag(stream, "header");

    Header *header = xmlParseHeader(stream);
    cache[filename] = header->clone();

    return header;
}

QString xmlPeekName(const QString &filename)
{
    read_xml_file(filename);
    while (!stream.atEnd())
    {
        stream.readNext();
        if (stream.isStartElement())
        {
            foreach (const auto &attr, stream.attributes())
                if (attr.name() == "name")
                    return attr.value().toString();
        }
    }
    return "";
    // TODO temporarily disabled exception
    throw std::logic_error(": No name is defined for this object: " + filename.toStdString());
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
        foreach (const auto &attr, stream.attributes())
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
                    project->addSheet(xmlParseSheet(stream));
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
        throw std::runtime_error("Unable to open XML file for writing: " + filename.toStdString());
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.setAutoFormattingIndent(4); // consider changing this to tabs

    stream.writeStartElement("project");
    stream.writeAttribute("title", project->getTitle());
    for (auto *sheet : project->getSheets())
        xmlWriteSheet(sheet, stream);
    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

Sheet *xmlParseSheet(QXmlStreamReader &stream)
{
    Sheet *sheet = new Sheet;

    try {
        // Parse the attributes
        foreach (const auto &attr, stream.attributes())
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
                else if (stream.name() == "var")
                    sheet->addVariable(xmlParseVariable(stream));
                else
                    sheet->addObject(xmlParseObject(stream));
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

    xmlWriteHeader(sheet->getHeader(), stream);

    for (const auto &var : sheet->getLocalVariables())
        xmlWriteVariable(var, stream);
    for (auto *obj : sheet->getObjects())
        xmlWriteObject(obj, stream);

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

Object *xmlParseObject(QXmlStreamReader &stream)
{
    Object *object = nullptr;

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
    else if (stream.name() == "linear-array")
        object = xmlParseLinearObjectArray(stream);
    else
        throw std::logic_error("Unknown object type: " + stream.name().toString().toStdString());

    auto objectName = stream.name();

    // TODO maybe the following part can be removed.
    // Eat up all remaining tokens related to this object
    while (!stream.atEnd() && !(stream.isEndElement() && stream.name() == objectName))
        stream.readNext();

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
    Line *line = new Line;
    xmlReadProperties(line, stream);
    return line;
}

void xmlWriteLine(Line *line, QXmlStreamWriter &stream)
{
    Line def; // Object with default values
    stream.writeStartElement("line");

    stream.writeAttribute("x1", line->getProperty("x1"));
    stream.writeAttribute("y1", line->getProperty("y1"));
    stream.writeAttribute("x2", line->getProperty("x2"));
    stream.writeAttribute("y2", line->getProperty("y2"));
    if (line->getLinewidth() != def.getLinewidth())
        stream.writeAttribute("lw", line->getProperty("lw"));

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

Rect *xmlParseRect(QXmlStreamReader &stream)
{
    Rect *rect = new Rect;
    xmlReadProperties(rect, stream);
    return rect;
}

void xmlWriteRect(Rect *rect, QXmlStreamWriter &stream)
{
    Rect def; // Object with default values
    stream.writeStartElement("rect");

    stream.writeAttribute("x", rect->getProperty("x"));
    stream.writeAttribute("y", rect->getProperty("y"));
    stream.writeAttribute("w", rect->getProperty("w"));
    stream.writeAttribute("h", rect->getProperty("h"));

    if (rect->getLinewidth() != def.getLinewidth())
        stream.writeAttribute("lw", rect->getProperty("lw"));

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

Text *xmlParseText(QXmlStreamReader &stream)
{
    Text *text = new Text;
    xmlReadProperties(text, stream);
    return text;
}

void xmlWriteText(Text *text, QXmlStreamWriter &stream)
{
    Text def; // Object with default values
    stream.writeStartElement("text");

    if (text->getText() != def.getText())
        stream.writeAttribute("text", text->getText());
    stream.writeAttribute("x", text->getProperty("x"));
    stream.writeAttribute("y", text->getProperty("y"));
    stream.writeAttribute("height", text->getProperty("height"));
    if (text->getFont() != def.getFont())
        stream.writeAttribute("font", text->getFont());

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

CompositeObject *xmlParseCompositeObject(QXmlStreamReader &stream)
{
    CompositeObject *obj = nullptr;

    QString objectName = stream.name().toString();

    foreach (auto attr, stream.attributes())
    {
        if (attr.name() == "from" && obj == nullptr)
        {
            // The content is taken from another file
            obj = parseCompositeObject(resolvePath(attr.value().toString()));
            obj->setSourceFile(attr.value().toString());
        }
    }

    if (obj == nullptr)
        obj = new CompositeObject;

    try
    {
        while (!stream.atEnd() && !(stream.isEndElement() && stream.name() == objectName))
        {
            stream.readNext();
            if (stream.isStartElement())
            {
                if (stream.name() == "var")
                    obj->addVariable(xmlParseVariable(stream));
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

CompositeObject *xmlParseFromDxf(QXmlStreamReader &stream)
{
    // Read the content of the "dxf" tag which is in base64 format
    QString content = stream.readElementText();
    auto arr = QByteArray::fromBase64(content.toUtf8(), QByteArray::Base64Encoding);

    return dxfParseCompositeObject(arr.toStdString());
}

LinearObjectArray *xmlParseLinearObjectArray(QXmlStreamReader &stream)
{
    float dx = 1, dy = 0;
    int count = 1;
    bool conversion_ok = true;
    foreach (auto attr, stream.attributes())
    {
        if (attr.name() == "dx")
            dx = attr.value().toFloat(&conversion_ok);
        else if (attr.name() == "dy")
            dy = attr.value().toFloat(&conversion_ok);
        else if (attr.name() == "count")
            count = attr.value().toUInt(&conversion_ok);
        else
            throw std::logic_error("Unknown attribute for linear object array: " + attr.name().toString().toStdString());
        if (!conversion_ok)
            throw std::logic_error("Linear object array attribute \"" + attr.name().toString().toStdString() + "\" is of invalid format");
    }
    CompositeObject *baseObj = xmlParseCompositeObject(stream);
    if (baseObj->size() == 1 && dynamic_cast<CompositeObject*>((*baseObj)[0]))
        baseObj = static_cast<CompositeObject*>((*baseObj)[0]);

    return new LinearObjectArray(baseObj, dx, dy, count);
}

///////////////////////////////////////////////////////////////////////////

Header *xmlParseHeader(QXmlStreamReader &stream)
{
    Header *header = nullptr;
    foreach (auto attr, stream.attributes())
    {
        if (attr.name() == "from" && header == nullptr)
        { // The header content is taken from another file
            header = new Header(xmlParseHeader(resolvePath(attr.value().toString())));
            // TODO how can I make it possible to get the header data from a dxf file
            header->setSourceFile(attr.value().toString());
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
            {
                if (stream.name() == "content-area")
                    header->setContentArea(*xmlParseRect(stream));
                else
                    header->append(xmlParseObject(stream));
            }
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

    // TODO do this for all other types of CompositeObject
    if (header->getSourceFile() != "") // The header is associated with a source file
        stream.writeAttribute("from", header->getSourceFile());
    else
        foreach (auto obj, *header)
            xmlWriteObject(obj, stream);

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

Component *xmlParseComponent(QXmlStreamReader &stream)
{
    Component *obj = nullptr;
    QPointF pos;
    bool conversion_ok = true;
    foreach (auto attr, stream.attributes())
    {
        if (attr.name() == "from")
        { // The content is taken from another file
            if (obj == nullptr)
            {
                obj = Component::absorb(parseCompositeObject(resolvePath(attr.value().toString())));
                obj->setSourceFile(attr.value().toString());
            }
        }
        else if (attr.name() == "x")
            pos.setX(attr.value().toFloat());
        else if (attr.name() == "y")
            pos.setY(attr.value().toFloat());
        //TODO enable this else
            //throw std::logic_error("Unknown component attribute");
        if (!conversion_ok)
            throw std::logic_error("Component attribute \"" + attr.name().toString().toStdString() + "\" is of invalid format");
    }

    if (obj == nullptr) // The content was not taken from a file
        obj = new Component;

    obj->setPos(pos);

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

    Component def; // Default component
    QPointF defPos = def.getPos(), pos = component->getPos();

    if (pos.x() != defPos.x())
        stream.writeAttribute("x", QString::number(pos.x()));
    if (pos.y() != defPos.y())
        stream.writeAttribute("y", QString::number(pos.y()));

    foreach (auto var, component->getVariables())
        xmlWriteVariable(var, stream);

    foreach (auto obj, *component)
        xmlWriteObject(obj, stream);

    stream.writeEndElement();
}

///////////////////////////////////////////////////////////////////////////

void xmlWriteVariable(const Variable &var, QXmlStreamWriter &stream)
{
    stream.writeStartElement("var");
    stream.writeAttribute("names", var.names.join(' '));
    stream.writeAttribute("value", var.value);
    stream.writeAttribute("desc", var.description);
    stream.writeEndElement();
}
