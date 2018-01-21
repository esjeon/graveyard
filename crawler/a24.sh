#!/bin/bash

cmd="$1"
arg="$2"

useragent="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36"

httpget() {
	wget -U "$useragent" -O - "$@"
}

list_vids() {
	aid="$1"
	url=$(echo -n 'aHR0cDovL2FuaTI0Lm9yZy9hbmlfbGlzdA==' | base64 -d)
	httpget "${url}/${aid}.html" | grep '<td' | egrep -o 'ani_view/[0-9]+' | cut -b10-
}

get_vid_url() {
	vid="$1"
	url1=$(echo -n 'aHR0cDovL3Rlc3QuYW5pMjQub3JnL2FuaV92aWRlbw==' | base64 -d)
	url2=$(echo -n 'Lmh0bWw/cGxheWVyPWJyb3dzZXI=' | base64 -d)
	httpget "${url1}/${vid}${url2}" | grep 'source' | grep -o "http://[^\"]*"
}

down_vid() {
	vid="$1"
	url="$(get_vid_url "$vid")"
	wget -U "$useragent" -O "${vid}.mp4" "$url"
}

case "${cmd}" in
	"list") list_vids "$arg" ;;
	"url") get_vid_url "$arg" ;;
	"down") down_vid "$arg" ;;
	"down-all")
		list_vids "$arg" |\
		while read vid; do
			down_vid "$vid"
			sleep 10m
		done
esac

