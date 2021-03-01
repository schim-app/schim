=====
schim
=====

SYNOPSIS
========

.. code-block:: none

   schim [--version] [--help]
   schim [<options>] [<command>]

By default, if no <command> is specified, **schim-editor(1)** is run.
This can be changed in the configuration (see **schim-config(1)**).

DESCRIPTION
===========

Schim is a modern electrical schematic editor with a focus on industrial
applications. Schim is most commonly used to create and edit schematics
using the graphical editor it provides. It also provides a very rich CLI
interface to automate a wide range of tasks.

OPTIONS
=======

`-v`\ , `--version`
   Prints the currently installed version of Schim.

`-h`\ , `--help`
   Prints the synopsis and a list of the most commonly used commands.

SCHIM COMMANDS
==============

schim-editor(1)
---------------

Launch Schim's graphical editor. This is the default command unless
changed in the configuration (see **schim-config(1)).**

schim-export(1)
---------------

Export schim projects into various formats.

schim-report(1)
---------------

Generate reports for schim projects.

schim-set(1)
------------

Change schim user settings.

schim-alias(1)
--------------

Create and edit aliases for schim commands.

schim-tweak(1)
--------------

Change the properties or contents of a project without using the
graphical editor.

REPORTING BUGS
==============

Report all bugs on `<https://github.com/HarisGusic/schim/issues>`_.

SEE ALSO
========

schim-config(1)

