===============
Getting Started
===============

.. toctree::

Dependencies
============

Build
-----

* C++11, Qt 5
* make
* `dxflib <https://qcad.org/en/90-dxflib>`_ (Library for reading/writing DXF files)

Documentation
-------------

* `Doxygen <https://www.doxygen.nl/index.html>`_
* `Sphinx-doc <https://www.sphinx-doc.org/en/master/>`_

  * `Breathe <https://breathe.readthedocs.io/en/latest/index.html>`_

Directory Structure
===================

.. literalinclude:: /_build/tree.txt
   :language: none

Conceptual Structure
====================

|startovw|

.. image:: /_build/html/dev/overview.svg
   :align: center
|br|
*Note:* An arrow from object A to B means that **A sees B**.

|endcollapse|

The project is divided into four conceptual groups, which are meant to be as
decoupled as possible:

- :ref:`Model`
- :ref:`Graphical user interface <GUI>`
- :ref:`File input/output <File IO>`
- :ref:`Command line interface`

The model contains a representation of a project and all its components that is
independent of the format it is exported to. Model classes are located in files
within the `src/model/` directory. **The model must know nothing about the other
groups.**

The graphical user interface contains everything that is related to representing
a project to the user in the graphical editor, i.e. in the application. As such,
it needs to have knowledge of the model. **It must not know about file
input/output or the command line interface.** GUI classes are located in the
`src/ui/` directory.

File input/output is used to generate files from models or vice versa. **It must
know about the model, but not about the other groups.** File IO classes are
located in the `src/fileio/` directory.

The command line interface provides an interface for quick operations on a
project from a terminal, without the need to open the full GUI application. **It
must know only about the model.** CLI classes are located in the `src/cli/`
directory.
