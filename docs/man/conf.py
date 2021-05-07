import glob
import sys
import os

project = 'Schim Manual' # Displayed on top of a manpage when viewed in man

man_pages = []

# Provides function get_description to load command descriptions for man pages
sys.path.insert(0, os.path.dirname(__file__))
from descriptions import man_descriptions

for f in glob.glob('schim*.rst'):
    man_pages.append((
        f[:-4],                     # source file (no extension)
        f[:-4],                     # output file (under output dir)
        man_descriptions[f[:-4]],   # description
        'Haris Gušić <harisgusic.dev@gmail.com>',   # author
        1,                                          # section
    ))
