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

