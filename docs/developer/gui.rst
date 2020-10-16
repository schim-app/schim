===
GUI
===

.. toctree::

Qt provides `QGraphicsView`_ and `QGraphicsScene`_ to visually represent
`QGraphicsItem`_-s. You should have at least a basic knowledge of these classes
if you wish to develop the GUI. This project doesn't use these classes directly.
Instead we use their subclasses: :ref:`SheetView`, :ref:`SheetScene` and
:ref:`GObject` respectively.

An object is visually represented as a :ref:`GObject`, which is a wrapper around
an :ref:`Object`. Derived classes follow the same naming convention. Namely, the
graphical class is named by prepending the corresponding model class name with
the letter 'G' (for graphical). For example, a :ref:`Line` is wrapped by
:ref:`GLine`.

.. image:: ../_build/doxygen/html/classGObject__inh.svg   
   :align: center

.. doxygenclass:: GObject

.. doxygenclass:: GCompositeObject

.. _QGraphicsView: https://doc.qt.io/qt-5/qgraphicsview.html
.. _QGraphicsScene: https://doc.qt.io/qt-5/qgraphicsscene.html
.. _QGraphicsItem: https://doc.qt.io/qt-5/qgraphicsitem.html
