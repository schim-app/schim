=================================
Welcome to Schim's documentation!
=================================

--------------------------
Schim = SCHematic IMproved
--------------------------

Schim is a tool used to create electrical schematics, mainly for industrial
applications. It aspires to become the standard open source tool for this
purpose.

An Open Source Tool
    ..																		

There is already a de facto standard application for drawing electrical diagrams
called EPLAN, but it is proprietary. Also, there is QElectroTech which is a
laudable attempt to make an open source alternative and it has been a very
useful tool to me. But, it has made a lot of design decisions which I do not
agree with and it lacks functionality. Also, it seems the bulk of its
development consists of translations these days. Another dealbreaker for me is
that its documentation is mainly in French, which I do not speak.

With The Best of IT
   ..

Schim aims to bring the best of IT to industry. This includes open source, the
`Unix philosophy <https://en.wikipedia.org/wiki/Unix>`_ - primarily the
"programs that work together" part, Git and any other tools you want. It
includes a rich CLI interface, because what good is a program if it can't be
automated using scripts. Also, a major pet-peeve of mine, virtually all software
used in industry is completely proprietary. This means that they mostly use
encrypted binary files. This is so irritating because you cannot use other tools
to edit files in these projects and can't utilize Git to its full potential.
Therefore, you are constrained by the feature-richness of the proprietary
software. Schim operates with files that are human readable and uses a project
directory structure, which allows you to tweak those files with any tools you
can muster.

Be Productive
   ..

It is also designed to maximize productivity. As the name might suggest, it also
has Vim-like key mappings! However, it can be used as a normal application
without those mappings.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   quick-start.rst
   contribute.rst
   manual/index.rst
   developer/index.rst

