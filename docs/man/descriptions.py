#!/usr/bin/env python3

# Return the description of the man page for the specified `command`
def get_description(command):

    man_descriptions = {
        'schim': 'An electrical schematic editor for the modern computer user',
        'schim-editor': "Launch Schim's graphical editor",
        'schim-export': 'Export schim projects into various formats',
        'schim-report': '',
        'schim-set': ''
        }

    return man_descriptions[command]
