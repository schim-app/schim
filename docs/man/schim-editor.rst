============
schim-editor
============

-------------------------------
Launch Schim's graphical editor
-------------------------------

:Author: Haris Gušić <harisgusic.dev@gmail.com>
:Date:   2020-12-08
:Copyright: TODO
:Version: TODO
:Manual section: 1
:Manual group: Schim Manual

SYNOPSIS
========

| schim editor [<options>] [<FILE>]
| schim editor [<options>] -

DESCRIPTION
===========

If a <FILE> is given, schim tries to open it as a project. The file should be of
the .sch format.

If '-' is listed as one of the arguments, the project will be read from
**stdin**, assuming that the contents are formatted as XML.

Otherwise, an empty new project is opened.

OPTIONS
=======

-h, --help
   Prints the synopsis and a list of the most commonly used options.
-r, --read-only
   The project will be open as read-only. Any attempt to save the project will
   fail.
-I <PATHS>, --include=<PATHS>
   <PATHS> will be searched when looking up symbols and components. <PATHS>
   are specified as a list of absolute or relative paths separated by commas.
   The entries can also be individual files. If a path contains a comma, it can
   be escaped using a **'\\'**.
--config-dir=<DIR>
   Load the configuration from <DIR> instead of the default directory. See
   also **schim-config(1)**.

SCHIM
=====
Part of the **schim(1)** suite.
