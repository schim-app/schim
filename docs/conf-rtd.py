#!/usr/bin/env python3

import subprocess

################
# Preparations #
################

# Scripts must be made executable on ReadTheDocs, but we just give full
# permissions to all files to prevent future headaches
subprocess.call('chmod -R 777 ./', shell=True)
subprocess.call('umask 000', shell=True)

# Add a tag so we can customize some rst files for ReadTheDocs
tags.add('ReadTheDocs')
# Confer [man_link]
exclude_patterns.remove('man')

############
# Building #
############

# Build doxygen and move the generated files to _build/html/doxygen
subprocess.call('make doxygen', shell=True)
# Tweak manpages for inclusion in the HTML version of the docs
subprocess.call('make prepare-man', shell=True)
# Move them to man/ so the resulting http link looks nicer [man_link]
subprocess.call('mv _intermediate/man/* man/', shell=True)
subprocess.call('make graphs', shell=True)

############################
# Debugging on ReadTheDocs #
############################

"""
# Only uncomment this section if something is going wrong on ReadTheDocs

# In the Sphinx documentation, this function is said to require three arguments.
# But when the third one is positional, an exception is raised.
# We don't use it anyway, so set its default value to None.
def build_finished_handler(app, docname, source=None):
    # Check if the correct files have been generated
    subprocess.call('ls -Rl', shell=True)

def setup(app):
    app.connect('build-finished', build_finished_handler)
"""

