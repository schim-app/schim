===
GUI
===

.. toctree::

.. important:: If you need to quickly find out where a graphical item is defined
   in the source code, go to
   
   .. centered:: :menuselection:`Help-->Toggle Developer Hints`

   in the application. This will display where the focused widget is defined.

The main window
===============

The class holds a static instance of ``MainWindow`` called ``instance``, which
allows other classes to access the main window.

.. tabs::

   .. tab:: Outline

      .. doxygenclass:: MainWindow
         :outline:
         :no-link:

   .. tab:: Details

      .. doxygenclass:: MainWindow
         :no-link:
         :undoc-members:
* `Full reference <../../doxygen/classMainWindow.html>`_
     ..

The schematic editor
====================

Qt provides `QGraphicsView`_ and `QGraphicsScene`_ to visually represent
`QGraphicsItem`_-s. You should have at least a basic knowledge of these classes
if you wish to develop the GUI. This project doesn't use these classes directly.
Instead we use their subclasses: :ref:`SheetView`, :ref:`SheetScene` and
:ref:`GObject` respectively, which we have defined.

GObject
-------

|startovw|

.. rubric:: Inheritance graph
.. image:: /_build/html/doxygen/classGObject__inherit__graph.svg
   :align: center

|br|

.. rubric:: Collaboration graph
.. image:: /_build/html/doxygen/classGObject__coll__graph.svg
   :align: center

|endcollapse|

An object is visually represented as a :ref:`GObject`, which is a wrapper around
an :ref:`Object`. Derived classes follow the same naming convention. Namely, the
graphical class is named by prepending the corresponding model class name with
the letter 'G' (for graphical). For example, a :ref:`Line` is wrapped by
:ref:`GLine`.

The following classes are derived from :ref:`GObject`:

* `GLine <../../doxygen/classGLine.html>`_
* `GRect <../../doxygen/classGRect.html>`_
* `GText <../../doxygen/classGText.html>`_
* `GCompositeObject <../../doxygen/classGCompositeObject.html>`_
* `GHeader <../../doxygen/classGHeader.html>`_

.. tabs::

   .. tab:: Outline

      .. doxygenclass:: GObject
         :outline:

   .. tab:: Details

      .. doxygenclass:: GObject
         :no-link:
         :undoc-members:
* `Full reference <../../doxygen/classGObject.html>`_
     ..

.. _QGraphicsView: https://doc.qt.io/qt-5/qgraphicsview.html
.. _QGraphicsScene: https://doc.qt.io/qt-5/qgraphicsscene.html
.. _QGraphicsItem: https://doc.qt.io/qt-5/qgraphicsitem.html

Vim mode
========

Vim mode is implemented in `src/ui/vim.h` and `src/ui/vim.cpp`.

.. todo:: Add more details.

.. tabs::

   .. tab:: Outline

      .. doxygennamespace:: Vim
         :outline:

   .. tab:: Details

      .. doxygennamespace:: Vim
         :no-link:
* `Full reference <../../doxygen/namespaceVim.html>`_
     ..
