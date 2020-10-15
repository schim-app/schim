=====
Model
=====

.. toctree::

Everything that exists on a sheet is represented by a model class. A model class
contains those attributes that are consistent regardless of the format in which
the object is displayed. For example, the model class of an object does not
contain information about whether the object is selected for editing, because
that is only relevant in the context of editing.

All objects are derived from the base class :ref:`Object`.

Object
======

.. doxygenclass:: Object
   :members:
   :protected-members:
   :private-members:
   :undoc-members:
