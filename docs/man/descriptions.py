#!/usr/bin/env python3

man_descriptions = {
    'schim': 'An electrical schematic editor for the modern computer user',
    'schim-editor': "Launch Schim's graphical editor",
    'schim-export': 'Export schim projects into various formats',
    'schim-report': '',
    'schim-set': ''
    }

if __name__ == '__main__':
    import sys
    print(man_descriptions[sys.argv[1]])
