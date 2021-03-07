

project = 'Schim Manual' # Displayed on top of a manpage when viewed in man

import glob

man_pages = []

# Provides function __get_description to load command descriptions for man pages
exec(open('descriptions.py').read())


for f in glob.glob('schim*.rst'):
    man_pages.append((
        f[:-4], # source file (no extension)
        f[:-4], # output file (under output dir)
        __get_description(f[:-4]), # description
        'Haris Gušić <harisgusic.dev@gmail.com>', # author
        1, # section
    ))
