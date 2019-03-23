#!/bin/sh

for i in $(../src/random_inrange); do
	if ! { [ $i -lt 100 ] && [ $i -ge 0 ]; }; then
		exit 1;
	fi
done
