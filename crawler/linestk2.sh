#!/bin/bash

set -e

page_url="$1"

if [[ -z "$page_url" ]]; then
	echo "usage: $(basename "$0") STORE-PAGE-URL" >&2
	exit 2
fi

html="linestk-$$.html.tmp"

wget "$page_url" -O "$html"

title="$(grep Item01Ttl "$html" | sed 's/ *<[^>]*>//g')"
mkdir "$title"
mv "$html" "$title"/
cd "$title"

# animation
grep -o 'https:\/\/[^";]*/sticker_animation@2x.png' "$html" | \
	while read url; do
		stkid="$(echo "$url" | grep -o '[0-9]\{2,\}')"
		wget "$url" -O "${stkid}-animation.png" || true
	done || true

# static
grep -o 'https:\/\/[^";]*/sticker@2x.png' "$html" | \
	while read url; do
		stkid="$(echo "$url" | grep -o '[0-9]\{2,\}')"
		wget "$url" -O "${stkid}.png" || true
	done || true

rm -f "$html"
