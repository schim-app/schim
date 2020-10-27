=======
File IO
=======

.. toctree::

Schim currently supports a custom XML file format for storing projects, sheets,
headers and objects. We are planning to support using DXF files as the source
files for symbols. However, the XML format will remain the officially supported
format.

The idea is to make the format human-readable as much as possible. This allows
for tweaks to the files using any text editor. That way, you are not confined to
merely the features that Schim provides. Another benefit of this approach is the
ability to use **git** to its full potential.

XML
===

The XML format uses the .xsym extension for defining a symbol and the .xproj
extension for defining a project. A sample project may look like the following:

.. literalinclude:: examples/project.xproj
   :language: xml

Every project file starts with a ``<project>`` tag. A project consists of sheets,
which are defined using ``<sheet>`` tags. Each sheet contains a header, which
is specified using the ``<header>`` tag. If no header is specified, the sheet
will use the default header.

The ``from`` attribute
----------------------

Any tag can use this attribute to take its content from another file. The tag
can also specify additional objects to include. For example, the following
content

.. code-block:: xml

   <header from="Default header">
      <line x1="5" y1="170" x2="292" y2="170" />
   </header>

will create a header from the default header, but it will contain an additional
line.

The attribute value is very flexible - it can be specified in a variety of
formats. It is interpreted in the following order:

* File path relative to project root
* Absolute file path
* File path relative to user directory
* File path relative to installation directory
* Name of the resource

.. todo:: Idea

   Add a global map of headers, so they do not have to be read from the file
   each time a new sheet is created.

Functions
---------

All the functions reside in the files ``src/fileio/xml.h`` and
``src/fileio/xml.cpp``. They are grouped into parse and write functions. Because
objects can be defined as child tags of a parent tag or as a standalone
document, there can be two versions of the parse function for each type of
object. To explain how the XML interface works, let's look at the parser for
``Header`` for example.

.. doxygenfunction:: xmlParseHeader(QXmlStreamReader &stream)
   :outline:

.. doxygenfunction:: xmlParseHeader(const QString &filename)
   :outline:

The first version takes an XML stream as its argument and extracts the header
from there. The function assumes that the stream will be at the start of the
header element after ``stream.readNext()`` is called. Otherwise, an exception is
thrown.

The second version takes a file name as its argument. The specified file
contains the header element. This version opens the file for reading and
constructs an XML stream. Then it calls the first version of the function to
process the stream.

The following lists all the functions that are defined for handling XML files.

.. doxygenfile:: src/fileio/xml.h


