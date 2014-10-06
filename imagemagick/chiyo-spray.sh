#!/bin/bash
# This script is used to create a in-game spray for Left 4 Dead 2.

each() {
	set -e
	[[ -n "$1" ]]; src="$1"; shift 1
	[[ -n "$1" ]]; dst="$1"; shift 1

	rm -rf "${dst}"; mkdir "${dst}"
	for file in $(ls "${src}/"); do
		[[ -f "${src}/${file}" ]] || continue
		convert "${src}/${file}" "$@" "${dst}/${file}" &
	done

	wait
}

crop() {
	each "$1" "crop" -crop 720x720+560+0 +repage
}
scale() {
	each "$1" "scale" -scale 50%
}

ani() {
	set -e
	[[ -n "$1" ]]; src="$1"; shift 1
	dst=ani

	rm "${dst}.gif"
	convert -dispose previous -alpha set -delay 0 \
		"${src}"/*.png "${dst}.gif"
}

pick() {
	step=4

	set -e
	[[ -n "$1" ]]; src="$1"; shift 1
	dst=pick

	rm -rf "${dst}"; mkdir "${dst}"

	cnt=${step}
	for file in $(ls "${src}/"); do
		if [[ ${cnt} -eq ${step} ]]; then
			cp "${src}/${file}" "${dst}/"
			cnt=1
		else
			cnt=$((cnt + 1))
		fi
	done
}

filt() {
	# 82,234,150 - bright cyan
	# 10,120,95 - blueish green
	# 59,116,90 - dark green (for shade)
	# 50,110,85 - dark green (for shade)
	# 141,245,245 - bright skyblue
	each "$1" "filt" \
		-fuzz 23% -transparent "rgb(82,234,150)" \
		-fuzz 13% -transparent "rgb(10,120,95)" \
		-fuzz 05% -transparent "rgb(59,116,90)" \
		-fuzz 06% -transparent "rgb(50,110,85)" \
		-fuzz 04% -transparent "rgb(141,245,245)" \

}

#filt() {
#	set -e
#	[[ -n "$1" ]]; src="$1"; shift 1
#	dst=filt
#
#	rm -rf "${dst}"; mkdir "${dst}"
#	for file in $(ls "${src}/"); do
#		./wand.sh 0,0 -t 24 -r outside -m transparent -c trans \
#			"${src}/${file}" "${dst}/${file}" &
#	done
#
#	wait
#}




function_exists() {
	declare -f -F $1 > /dev/null
	return $?
}

if function_exists "$1"; then
	"$@"
	echo "Done"
else
	echo "invalid command"
fi

