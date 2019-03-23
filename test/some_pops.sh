#!/bin/sh

(
pids=""
for p in $(seq 20 200); do
	i=$((p % 10))
	if [ $((i + 1)) -eq 0 ]; then
		for pid in $pids; do
			wait $pid || exit 1
		done
		pids=""
	fi
	../src/genetic -qp $p 'evan is cool' &
	pids="$pids $!"
done
)
