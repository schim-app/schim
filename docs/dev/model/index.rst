=====
Model
=====

.. toctree::

Everything that exists in a project is represented by a model class, including
the project itself. A model class contains those attributes that remain the
same regardless of the format in which something is accessed. For example, the
model class of an object does not contain information about whether the object
is selected for editing, because that is only relevant in the context of the
graphical editor.  All model classes reside in files under the `src/model/`
directory or one of its subdirectories.

Project
=======

A project is nothing more than a collection of sheets (of paper) and some
configured parameters.

.. tabs::

   .. tab:: Outline

      .. doxygenclass:: Project
         :outline:
                  
   .. tab:: Details

      .. doxygenclass:: Project
         :no-link:
         :undoc-members:

* `Full reference <../../doxygen/classProject.html>`_
     ..

Sheet
=====

.. tabs::

   .. tab:: Outline

      .. doxygenclass:: Sheet
         :outline:

   .. tab:: Details

      .. doxygenclass:: Sheet
         :no-link:
         :undoc-members:

* `Full reference <../../doxygen/classSheet.html>`_
     ..

Object
======

|startovw|

.. rubric:: Inheritance graph
.. image:: /_build/html/doxygen/classObject__inherit__graph.svg
   :align: center
.. rubric:: Collaboration graph
.. image:: /_build/html/doxygen/classObject__coll__graph.svg
   :align: center

|endcollapse|

All objects are derived from the base class Object.

.. tabs::

   .. tab:: Outline

      .. doxygenclass:: Object
         :outline:

   .. tab:: Details

      .. doxygenclass:: Object
         :no-link:
         :undoc-members:

* `Full reference <../../doxygen/classObject.html>`_
     ..

Primitive objects
=================

Objects that directly inherit from the class :ref:`Object` (apart from
:ref:`CompositeObject<Composite objects>`) are primitive objects. Currently, the
following primitive objects are defined:

* `Line <../../doxygen/classLine.html>`_
* `Rect <../../doxygen/classRect.html>`_
* `Text <../../doxygen/classText.html>`_

Composite objects
=================

Objects like electrical devices consist of multiple primitive objects such as
lines, rectangles, etc. Such objects are called composite objects. They are all
derived from the class :ref:`CompositeObject<Composite objects>`, which in turn
inherits :ref:`Object`.

.. tabs::

   .. tab:: Outline

      .. doxygenclass:: CompositeObject
         :outline:

   .. tab:: Details

      .. doxygenclass:: CompositeObject
         :no-link:
         :undoc-members:

* `Full reference <../../doxygen/classCompositeObject.html>`_
     ..

Variables
=========

For familiarity with the concept of variables, see :ref:`manual_variables` in
the user manual.

.. tabs::

   .. tab:: Outline

      .. doxygenstruct:: Variable
         :outline:

   .. tab:: Details

      .. doxygenstruct:: Variable
         :no-link:
         :undoc-members:

* `Full reference <../../doxygen/structVariable.html>`_
     ..

