#!/bin/bash
#
# httpserv.sh - a simple netcat-based http server
#               easy to customize
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
#

http_dummy() {
    read req file proto
    echo -ne "HTTP/1.1 200 OK\r\n"
    echo -ne "Content-Type: text/html\r\n"
    echo -ne "Content-Length: 34\r\n"
    echo -ne "\r\n"
    echo -ne "<html><h1>Hell World</h1></html>\r\n"
}

http_echo() {

    read req file proto

    ln="$(echo -ne '\r\n')"
    req="$req $file $proto"
    while read line; do
        req="${req}${line}"
        if [[ "${line}" = "${ln}" ]]; then
            break
        fi
    done

    if [[ "${file}" = "/quit" ]]; then
        resp="<html><h1>Quit</h1><pre>${req}</pre></html>"
    else
        resp="<html><pre>${req}</pre></html>"
    fi

    len="$(echo -ne "${resp}" | wc -c)"

    echo -ne "HTTP/1.1 200 OK\r\n"
    echo -ne "Content-Type: text/html\r\n"
    echo -ne "Content-Length: ${len}\r\n"
    echo -ne "\r\n"
    echo -ne "${resp}"

    if [[ "${file}" = "/quit" ]]; then
        return 1
    fi
}

declare -fx http_echo

while [[ $? -eq 0 ]]; do
    netcat -lp9980 -c "bash -c http_echo"
done

