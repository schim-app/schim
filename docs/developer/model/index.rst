=====
Model
=====

.. toctree::

Everything that exists in a project is represented by a model class, including
the project itself. A model class contains those attributes that are consistent
regardless of the format in which something is accessed. For example, the model
class of an object does not contain information about whether the object is
selected for editing, because that is only relevant in the context of the
graphical editor.  All model classes reside in files under the ``src/model/``
directory or one of its subdirectories.

Project
=======

A project is nothing more than a collection of sheets (of paper) and some
configured parameters.

.. doxygenclass:: Project

Sheet
=====

.. doxygenclass:: Sheet

Object
======

All objects are derived from the base class Object.

.. doxygenclass:: Object
   :protected-members:
   :private-members:
   :undoc-members:

.. image:: /_build/doxygen/html/classObject__inh.svg
   :align: center

Primitive objects
=================

Objects that directly inherit from the class :ref:`Object` (apart from
:ref:`CompositeObject`) are primitive objects. Currently, the following primitive
objects are defined:

.. toctree::
   :maxdepth: 1

   objects/line.rst
   objects/rect.rst
   objects/text.rst

Composite objects
=================

Objects like electrical devices consist of multiple primitive objects such as
lines, rectangles, etc. Such objects are called composite objects. They are all
derived from the class :ref:`CompositeObject`, which in turn inherits
:ref:`Object`.

The core differentiator between primitive and composite objects is that
composite objects have is that they contain a list (by inheriting QList as well)
of objects they are composed of.

.. doxygenclass:: CompositeObject

Variables
=========

For familiarity with the concept of variables, see :ref:`variables_manual` in
the user manual.

.. doxygenstruct:: Variable
