#!/bin/sh
# xclick8.sh: click x infinity

while true; do
	echo "ButtonPress 1"
	echo ButtonRelease 1
	sleep 0.05
done | xmacroplay :0 >/dev/null 2>/dev/null

