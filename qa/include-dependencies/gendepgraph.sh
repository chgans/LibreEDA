#!/bin/sh

set -e

here=$(dirname $0)

if [ $# -ne 0 ];
then
    src="$*"
else
    src=src/plugins/symboleditor
fi

out=include-dependencies

echo "Analysing dependencies ..."
perl $here/cinclude2dot \
     --exclude '.*third.*' \
     --quotetypes quote \
     --groups \
     --include $src \
     --merge directory \
     --src $src > $out.dot

# A3: 29.7x42.0 mm = 11.69x16.43 in
# 'sfdp' doesn't work for me and 'patchwork' looks ugly and useless
echo "Generating SVG ..."
for engine in dot neato twopi circo fdp; 
do
    dot -Gsize="11.69,16.53" \
	-K $engine \
	-Tsvg \
	$out.dot > $out-$engine.svg
done

# gwenview $out-dot.svg
