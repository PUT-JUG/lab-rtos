#!/bin/bash

set -e

cd src

if [[ "$OSTYPE" == "darwin"* ]]; then
	SED=( 'sed' '-i' '' )
else
	SED=( 'sed' '-i' )
fi

find . -type d | while read d ; do
	mkdir -p "../public/$d"
done

find . -iname "*.md" | while read f ; do
	title=`basename "$f"`
	grip --export --title "${title%.*}" "$f" "../public/${f%md}html"
	"${SED[@]}" 's/\/__\/grip\/static\//..\/static\//g' "../public/${f%md}html"
done