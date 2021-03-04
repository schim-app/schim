#######################
# Project information #
#######################

project = 'Schim'
copyright = '2021, Haris Gušić'
author = 'Haris Gušić'

#########################
# General configuration #
#########################

extensions = [
        'sphinx.ext.todo'
]

exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

todo_include_todos = True

html_theme = 'sphinx_rtd_theme'

default_role = 'envvar' # Like :code: role, but the text is black
