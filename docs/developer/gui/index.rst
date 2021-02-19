===
GUI
===

.. toctree::

The main window
===============

The class holds a static instance of ``MainWindow`` called ``instance``, which
allows other classes to access the main window.

.. tabs::

   .. tab:: Outline

      .. doxygenclass:: MainWindow
         :outline:
         :no-link:
   .. tab:: Detailed description

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

.. image:: /_build/doxygen/html/classGObject__inh.svg
   :align: center

.. tabs::

   .. tab:: Outline

      .. doxygenclass:: GObject
         :outline:
   .. tab:: Detailed description

      .. doxygenclass:: GObject
         :no-link:
         :undoc-members:
* `Full reference <../../doxygen/classGObject.html>`_
     ..

.. _QGraphicsView: https://doc.qt.io/qt-5/qgraphicsview.html
.. _QGraphicsScene: https://doc.qt.io/qt-5/qgraphicsscene.html
.. _QGraphicsItem: https://doc.qt.io/qt-5/qgraphicsitem.html
