#!/bin/bash

for file in *.rs; do
	rustc --edition=2018 "$file"
done

