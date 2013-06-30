#!/bin/bash

echo '=============================='
echo ' Dosa'
echo '=============================='
echo

for i in `seq 1 10`; do
	time ./dosa
done

echo '=============================='
echo ' Cosa'
echo '=============================='
echo

for i in `seq 1 10`; do
	time ./cosa
done
