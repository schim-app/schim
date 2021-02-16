#!/usr/bin/env bash

# Add NAME section for man page when displayed as HTML.
# The source rst files for the man pages do not include this section -- it is added
# automatically based on the configured command descriptions in
# 'docs/man/descriptions.py'

# Input:
#   1. rst source file for the manpage
#   2. destination directory

src="$1"
dest="$2"

# File name without extension (e.g. file <path>/schim.1 gives <path>/schim)
noextension="${src/.rst/}"
# Name of the command (e.g. file <path>/schim.1 gives schim)
name="$(basename "$noextension")"

# Create a sequence of # of the same length as the command name
hashes="$(echo $name | sed 's/./=/g')"
# Path to descriptions.py file
desc_path="$(dirname "${BASH_SOURCE[0]}")/../man/descriptions.py"
description="$(python3 -c "exec(open('$desc_path').read()); print(__get_description('$name'))")"

head -4 "$src" > "$dest/$name.rst"
echo -e "NAME\n====\n\n    $name - $description\n" >> "$dest/$name.rst"
tail +5 "$src" >> "$dest/$name.rst"
