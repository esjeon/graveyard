#!/bin/bash
# Haha, this works, too!

useragent="Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0"
target='/WEBSTORE/icon_198x278.png'

page="$1"
[[ -z "${page}" ]] && echo "Usage: $0 shop-page-url" && exit

get() {
	wget -U "${useragent}" -o /dev/null "$@"
}

get_theme_url() {
	get -O - "${page}" |\
	grep -m1 -o "https://.*${target}" |\
	sed 's/WEBSTORE\/.*\.png/IOS\/theme.zip/'
}

theme="$(get_theme_url)"

wget -O "theme.zip" "${theme}"

mkdir theme-$$
cd theme-$$

mv "../theme.zip" .
7z x "theme.zip"

cd ..

