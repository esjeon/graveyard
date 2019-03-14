#!/bin/bash

db="$( ls ${HOME}/.mozilla/firefox/*.default/places.sqlite )"
sql=sqlite3

cmd="$1"
shift 1

case "${cmd}" in
q)
	${sql} "${db}" "$@"
	;;
bak)
	cp -v "${db}" ./places.sqlite.bak
	;;
t)
	# search title
	# t {query}
	${sql} "${db}" "select * from moz_bookmarks where title like '$@' ;"
	;;
rt)
	# re-title
	# rt [-dry] {query} {regex}
	[[ "$1" = "-dry" ]] && dry="dry" && shift 1
	query="$1"
	regex="$2"
	${sql} "${db}" "select id,title from moz_bookmarks where title LIKE \"${query}\"" \
		> /tmp/$$-result
	while read line; do
		id="$( echo "$line" | cut -d "|" -f 1 )"
		title="$( echo "$line" | cut -d "|" -f 2- )"
		nutitle="$( echo "$title" | sed "${regex}")"
		if [ "${nutitle}" != "${title}" ]; then
			echo "${title} -> ${nutitle}"
		fi
		[[ -z "${dry}" ]] && \
			${sql} "${db}" "update moz_bookmarks set title=\"${nutitle}\" where id=$id ;"
	done < /tmp/$$-result
	;;
rl)
	# re-link
	# rl [-dry] {query} {regex}
	[[ "$1" = "-dry" ]] && dry="dry" && shift 1
	query="$1"
	regex="$2"
	${sql} "${db}" "select P.id,url from moz_bookmarks as B join moz_places as P where B.fk = P.id" \
		| egrep "${query}" > /tmp/$$-result
	while read line; do
		id="$( echo "$line" | cut -d "|" -f 1 )"
		link="$( echo "$line" | cut -d "|" -f 2- )"
		nulink="$( echo "$link" | sed "${regex}")"
		if [ "${nulink}" != "${link}" ]; then
			echo "${link} -> ${nulink}"
		fi
		[[ -z "${dry}" ]] && \
			${sql} "${db}" "update moz_places set url=\"${nulink}\" where id=$id ;"
	done < /tmp/$$-result
	;;
*)
	echo "invalid command: $1"
esac

rm -f /tmp/$$-*

