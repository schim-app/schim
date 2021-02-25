# Some global variables
import os
# Indicates whether this is built on ReadTheDocs
READTHEDOCS = os.environ.get('READTHEDOCS', False)

#########################
# Project configuration #
#########################

project = 'Schim'
copyright = '2021, Haris Gušić'
author = 'Haris Gušić'

#########################
# General configuration #
#########################

extensions = [
        'sphinx.ext.todo', 'breathe', 'sphinx.ext.autosectionlabel',
        'sphinx_tabs.tabs'
]

# Note: the man/ directory has its own conf.py and is not built by this one
exclude_patterns = ['_build', 'man', 'inc', 'Thumbs.db', '.DS_Store']
todo_include_todos = True

##################
# Breathe plugin #
##################

breathe_projects = { "Schim": "_build/doxygen/xml/" }
breathe_default_project = "Schim"
# Stop breathe from ignoring main.cpp
breathe_implementation_filename_extensions = []
breathe_default_members = ('members')

primary_domain = 'cpp'
highlight_language = 'cpp'

################
# HTML Options #
################

html_theme = 'sphinx_rtd_theme'
html_favicon = '../res/img/icon.svg'

##########
# Markup #
##########

smartquotes = False # Do not display '--' as long dashes
default_role = 'envvar' # Like :code: role, but the text is black

rst_prolog = ''
# Setup snippets that will be included in all rst files
exec(open('./include.py').read())

##########################
# Additional build steps #
##########################

# ReadTheDocs doesn't use Make -- it builds directly using sphinx
if READTHEDOCS:
    exec(open('./conf-rtd.py').read())

