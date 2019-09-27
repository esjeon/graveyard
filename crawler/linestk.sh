#!/bin/bash
# This does something, uh, useful... Yes, it does. It's just that you don't
# know what this does.

useragent="Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0"
target='/LINEStorePC/main.png'

page="$1"
[[ -z "${page}" ]] && echo "Usage: $0 shop-page-url" && exit

get() {
	wget -U "${useragent}" -o /dev/null "$@"
}

get_sticker_url() {
	get -O - "${page}" |\
	grep -m1 -o "https://.*${target}" |\
	sed 's/LINEStorePC\/main.png/iphone\/stickers@2x.zip/'
}

has_animation() {
	grep -o 'hasAnimation":true' productInfo.meta &>/dev/null
	return $?
}

get_animation_url() {
	id="$1"
	printf 'https://stickershop.line-scdn.net/stickershop/v1/sticker/%d/iPhone/sticker_animation@2x.png;compress=true' "$id"
}

sticker="$(get_sticker_url)"

wget -O "stickers@2x.zip" "${sticker}"

mkdir stickers-$$
cd stickers-$$

7z x "../stickers@2x.zip"
rename -v '@2x' '' *
rm -vf *_key*.png tab_*.png

if has_animation; then
	# HACK: list sticker ids
	ls [0-9]*.png |\
		while read file; do
			id="${file%.png}"
			output="${id}-animation.png"
			wget "$(get_animation_url "$id")" -O "$output"
		done
fi

rm -vf productInfo.meta
mv "../stickers@2x.zip" .

cd ..

