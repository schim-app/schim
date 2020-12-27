===========
User Manual
===========

.. toctree::

Getting Started
===============

...

Whenever you are unsure what something does, try dragging your mouse over it and
pressing **Shift+F1**. This will open a help page for that specific widget, if
one exists. This will work for many widgets. If you press only **F1**, a
general help page will open.

Opening a project
-----------------

The workspace
-------------

Inserting objects
-----------------

The most straight-forward way to insert an object is by clicking the buttons on
the tool bar. Afterwards, clicking on the sheet determines the object's position
in the sheet. Some primitive objects require multiple clicks to be placed. For
example, a line requires two clicks to specify its first and second endpoints,
respectively.

If a component is selected, a popup appears asking the user for the type of
component to be inserted. The popup provides a rich auto-completion feature.
This popup can be used to insert primitive objects as well. The auto-completion
popup has many more features. Have a look at :ref:`auto_completion`.

Another way to insert a component is to find it in the component browser (TODO
name?) and simply drag it into the sheet. We recommend that you get familiar
with the auto-completion feature because it is much more efficient.
Keyboard-centric users will especially love this feature.

Changing a sheet
----------------

Editing and creating components
-------------------------------

.. _auto_completion:

Component auto-completion
=========================

.. todo:: chapter name?

This feature is activated when you click on *Insert component* in the tool bar
or by pressing the appropriate keyboard shortcut. It has many features:

Completion by component name
----------------------------

When you enter text into the text box, Schim will first try to match any
component names. For example, if you enter ``coi``, Schim will try to complete
it to ``Coil``. By default, this type of completion is case-insensitive.

Completion by file name
-----------------------

If you enter part of a file name that defines a component, Schim will try to
complete it. The files are searched in the usual paths. By default, this type of
completion uses smart-case. This means that if the user enters only small
letters, the match will be case-insensitive; if the user enters text with
capital letters, the match will be case-sensitive.

.. todo:: Path specification

   Make a separate chapter defining ways a file path can be specified.

Generating reports
==================

.. _variables_manual:

Variables
=========

What if you want to change the position of a component and that position is
referenced from elsewhere in the sheet? You would have to go to each of those
places and manually change the text to show the new position of the component.
That's where variables come in.

A variable is referenced by ``%name`` or ``%{name}``, where ``name`` is the name
of the variable. So, in any object that displays text, any occurence of
``%name`` or ``%{name}`` is replaced with the value of the variable named
``name``. The ``%{name}`` should be used when the variable contents should be
immediately followed by non-whitespace characters. For example, if the variable
``%test`` has a value of *sch*, then the string ``%{test}im`` will be displayed
as *schim*.  Had the curly braces been omitted, then the value of the variable
``%testim`` would have been displayed.

Each occurrence of ``%name`` or ``%{name}`` will be substituted with the
corresponding value. If a variable is undefined it is assumed to have an empty
string as its value.

If you wish to display a ``%`` character, you should escape it with another
``%``. I.e. ``%%`` is displayed as *%*.

There are a few built-in variables:

+-------------+---------+----------------------------------------+
| Name        | Aliases | Description                            |
+=============+=========+========================================+
| designation | d dt    | Standard component identifier          |
+-------------+---------+----------------------------------------+
| function    | f       | The function group of the component    |
+-------------+---------+----------------------------------------+
| location    | l       | The mounting location of the component |
+-------------+---------+----------------------------------------+
| name        | n       | The component name                     |
+-------------+---------+----------------------------------------+
| page        | p pg    | The current page                       |
+-------------+---------+----------------------------------------+
| author      | a       | The author of the project              |
+-------------+---------+----------------------------------------+
| date        | D       | The export date                        |
+-------------+---------+----------------------------------------+
| page_total  | pt      | The total number of sheets             |
+-------------+---------+----------------------------------------+

The user can define any number of additional variables. Each variable has the
following properties:

* Name - the name with which to reference it
* Aliases - a list of abbreviations separated by spaces
* Value - the value with which to replace each occurence of ``%name``
* Description - useful for better organization, but can be omitted

Advanced topics
===============

.. toctree::
   :maxdepth: 1

   file-formats.rst
   cli.rst
