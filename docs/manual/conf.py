# -- Project information -----------------------------------------------------

project = 'Schim'
copyright = '2021, Haris Gušić'
author = 'Haris Gušić'

# -- General configuration ---------------------------------------------------

extensions = [
        'sphinx.ext.todo', 'sphinx.ext.autosectionlabel'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

todo_include_todos = True

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'

html_static_path = ['_static']
