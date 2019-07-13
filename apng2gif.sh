#!/bin/sh

set -e

if ! (which apngdis && which convert) &>/dev/null; then
	echo "imagemagick and apngdis are required to run this script" >&2
	exit 1
fi

TMP="$(mktemp -d -p /tmp)"
trap "{ rm -rf '$TMP'; }" EXIT

input="$1"
output="${input%.png}.gif"

cp "$input" "$TMP/input.png"
apngdis "$TMP/input.png" "frame-"
convert -delay 1 -alpha "set" -dispose "previous" "$TMP/frame-"*.png "$TMP/output.gif"
cp "$TMP/output.gif" "$output"

