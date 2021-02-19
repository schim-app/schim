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

`src/fileio/xml.h`, `src/fileio/xml.cpp`
   ..

For information on how an XML file is structured, see :ref:`manual_xml` in the
user manual.

Functions
---------

They are grouped into parse and write functions. Because objects can be defined
as child tags of a parent tag or as a standalone document, there can be two
versions of the parse function for each type of object. To explain how the XML
interface works, let's look at the parser for ``Header`` for example.

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

