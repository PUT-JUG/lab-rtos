#!/bin/bash

set -e

git ls-files --others "*html" | while read f ; do
	rm "$f"
done

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
	touch -r "$f" -c -m "../public/${f%md}html"
done