
# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

# -- Project information -----------------------------------------------------

project = 'Schim'
copyright = '2020, Haris Gušić'
author = 'Haris Gušić'

# -- General configuration ---------------------------------------------------

extensions = [
        'sphinx.ext.todo', 'breathe', 'sphinx.ext.autosectionlabel'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

todo_include_todos = True

# -- Breathe plugin ----------------------------------------------------------

breathe_projects = { "Schim": "_build/doxygen/xml/" }
breathe_default_project = "Schim"
# Stop breathe from ignoring main.cpp
breathe_implementation_filename_extensions = []
breathe_default_members = ('protected-members', 'undoc-members')

primary_domain = 'cpp'
highlight_language = 'cpp'

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'
html_favicon = '../img/icon.svg'

html_static_path = ['_static']
