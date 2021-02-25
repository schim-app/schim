#!/bin/bash

# Print the project source tree

cd "$(dirname "${BASH_SOURCE[0]}")"

cd ../..

# The root dir of the project
source_dir="$PWD"

# In this temporary directory we will create a directory tree that
# imitates the one in the project.
tmpdir=/tmp/"$(shuf -er -n20 {A..Z} {a..z} {0..9} | awk '{printf "%s", $0}')"
mkdir -p "$tmpdir"

# All the files and directories that are in the git repo with the $tmpdir prefix
files="$(git ls-tree -r master | awk '{print $4}' | sed 's:\(.*\):'"$tmpdir"'/\1:')"
# Create all the directories
mkdir -p $(dirname $files)
cd "$tmpdir"

mkdir -p "$source_dir/docs/_build"

tree -d | head -n -2 | tail -n +2 | sed 's/.\{4\}//' \
    > "$source_dir/docs/_build/tree.txt"

rm -rf "$tmpdir"

