#!/bin/bash

for file in *.rs; do
	rm -v $(basename "$file" .rs)
done



