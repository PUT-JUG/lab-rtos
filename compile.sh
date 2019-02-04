#!/bin/bash

set -e

cd src

# find . -iname "*.png" | while read f ; do
# 	gm convert -border 1 -bordercolor black "$f" "../public/$f"
# done

find . -iname "*.md" | while read f ; do
	title=`basename "$f"`
	grip --export --title "${title%.*}" "$f" "../public/${f%md}html"
	sed -i "" 's/\/__\/grip\/static\//static\//g' "../public/${f%md}html"
done