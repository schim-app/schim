Command Line Interface
======================

.. On ReadTheDocs we want the prepared manpages to be in the man/ directory so
   the http link looks nicer (otherwise it would include _intermediate)

.. only:: ReadTheDocs

   .. toctree::
      :maxdepth: 1

      man/schim.rst
      man/schim-editor.rst
      man/schim-export.rst
      man/schim-report.rst
      man/schim-set.rst

.. TODO we get a warning here because ../_intermediate/man/ does not exist
   although none of the conditions (not ReadTheDocs and not qthelp) are
   satisfied. This is probably a bug with Sphinx

.. only:: not ReadTheDocs and not qthelp

   .. toctree::
      :maxdepth: 1

      _intermediate/man/schim.rst
      _intermediate/man/schim-editor.rst
      _intermediate/man/schim-export.rst
      _intermediate/man/schim-report.rst
      _intermediate/man/schim-set.rst
