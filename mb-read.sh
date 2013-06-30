#!/bin/bash
# mb-read - just an impractical old garbage
#           uses fifo, exception handler, GUI, and event loop
#
# Copyright © 2013 Eon S. Jeon <esjeon@live.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the “Software”), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

COMIX=${COMIX:-"mcomix -fmdh"}
COLLECT=${COLLECT:-"collect"}
D=${PREFIX:-"/tmp/mb-$$"}


######
###### Initialize
######

set -m
exec 2>/dev/null

### tmp directory
mkdir -p "$D"
mkfifo "$D/event"
mkfifo "$D/history"
mkfifo "$D/book"

### file descriptors
exec 7<>"$D/event"
exec 8<>"$D/history"
exec 9<>"$D/book"

### initial run
echo next >&7 &

### trap
function cleanup () {
    for pid in `jobs -p`; do
        kill -TERM -"${pid}"
    done
    wait

    exec 7<&- 7>&-
    exec 8<&- 8>&-
    exec 9<&- 9>&-

    rm -rf "$D";
}
trap "cleanup" EXIT

### working directory
cd "${COLLECT}"



######
###### Main
######

### Generate Book List
ls | sort -R >&9 &


### GUI
(
    yad --text-info --tail            \
        --button=Jobs:'echo jobs'     \
        --button=Delete:'echo delete' \
        --button=Quit:'echo quit'     ;
    echo quit
) <&8 >&7 &


### Event Loop
while read event; do
    case "${event}" in
        delete)
            yad --text "Do you really want to remove\n'${book}'" \
                --button=Yes:0 --button=No:1
            if [[ $? -eq 0 ]]; then
                rm "${book}"
                kill -TERM -`jobs -p '( ${COMIX}'`
                echo next >&7
            fi
            ;;
        next)
            read book <&9
            echo "${book}" >&8
            ( ${COMIX} "${book}"; echo next >&7 ) &
            ;;
        quit) break ;;
        jobs) jobs ;;
    esac
done <&7

