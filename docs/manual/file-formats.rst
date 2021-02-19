============
File formats
============

.. toctree::

Schim's main file format for storing projects, symbols and templates is XML.
Project files use the extension .xproj whereas symbol files use the .xsym
extension. Other than XML, Schim projects and symbols can be exported
to/imported from DXF (.dxf) and exported to the following formats:

- Portable Document Format ( `.pdf` )
- Common image formats ( `.jpg`, `.png`, `.svg` )

.. _manual-xml:

XML
===

A project file may look like the following:

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
line which is specified in the body of the ``<header>`` tag.

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

