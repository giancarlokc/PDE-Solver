#!/bin/bash

SIZES="500 512 1022 1024 1026 1512 2046 2048"

# Remove previous test files
echo "Removing previous test files..."
rm -f out.dat

# Create test file
touch out.dat

echo "Starting tests..."
for testSize in $SIZES
do
	ARRAY=()
	echo "Running size:$testSize"

	# Iterate for each test and save the smallest time
	MIN=`./../pdeSolver -nx $testSize -ny $testSize -i 1`;
	for i in {1..10}
	do
   		OUTPUT=`./../pdeSolver -nx $testSize -ny $testSize -i 10`;

   		# Check if the current time is the smallest yet
   		if ! echo "$MIN $OUTPUT -p" | dc | grep > /dev/null ^-; then
		  MIN=$OUTPUT;
		fi
	done

	echo "$testSize | $MIN" >> out.dat

done

echo "Running plot..."
touch showgraph.txt
echo "set title \"3D surface from a grid (matrix) of Z values\"" >> showgraph.txt
echo "set datafile separator \"|" >> showgraph.txt
echo "plot \"out.dat\" with lines" >> showgraph.txt

gnuplot --persist showgraph.txt

#rm -f out.data
#rm -f solution.txt
#rm -f showgraph.txt