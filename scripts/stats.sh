#!/usr/bin/env bash

shopt -s extglob

cd $(dirname "${BASH_SOURCE[0]}")

args="$(echo ../src/?* | tr ' ' '\n')"
args="$(echo "$args" | sed 's/[^ ]*dxflib[^ ]*//g' | grep -v '.user')"

cloc -v $args
