============
schim-export
============

------------------------------------------
Export schim projects into various formats
------------------------------------------

:Author: Haris Gušić <harisgusic.dev@gmail.com>
:Date:   2020-12-08
:Copyright: TODO
:Version: TODO
:Manual section: 1
:Manual group: Schim Manual

SYNOPSIS
========

| schim export [<options>] [<INPUT-FILE>]
| schim export [<options>] -

DESCRIPTION
===========

The project from the input is converted to the format specified using --format.

If <INPUT-FILE> is given, the input project is taken from that file.

If '-' is listed as one of the arguments, the project will be read from
**stdin**, assuming that the contents are formatted as XML.

If '-o' is listed as one of the arguments, the output is written to the file
supplied to that option. Otherwise, the exported content is written to
**stdout**.

OPTIONS
=======

-h, --help
   Prints the synopsis and a list of the most commonly used options.
-f <FORMAT>, --format=<FORMAT>
   Set the output format. Can be one of: pdf, png, svg. By omitting this option,
   pdf is automatically used.
-o <FILE>, --output=<FILE>
   The output is written to <FILE>.
-r <SHEETS>, --range=<SHEETS> 
   The range of the document to be exported. <SHEETS> can be specified as a list
   of comma-separated page numbers (e.g. '1,2,4'), a range (e.g. '2-5'), or a
   combination of both (e.g. '1-3,5,7,9-11').
