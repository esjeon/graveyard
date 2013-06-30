#!/bin/bash

echo '=============================='
echo ' Mysqrt'
echo '=============================='
echo

for i in `seq 1 10`; do
	time ./mysqrt
done

echo '=============================='
echo ' sqrt'
echo '=============================='
echo

for i in `seq 1 10`; do
	time ./sqrt
done
