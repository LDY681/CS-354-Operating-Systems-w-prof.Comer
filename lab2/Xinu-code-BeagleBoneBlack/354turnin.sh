#!/bin/bash

# A turnin script for CS354. Please run this in the directory containing
# your Xinu-code-BeagleBoneBlack directory (named as such) with the lab 
# name as an argument ie: ./354turnin.sh lab0.

function turnin-project {
	# Create temp turnin-dir and copy relevant files
	mkdir -p turnin-dir
	cd Xinu-code-BeagleBoneBlack
	cp -r `ls -A | egrep -v "cross_compiler|compile"` ../turnin-dir
	cd ..

	# Turn in the project and remove temp directory
	turnin -c cs354 -p $1 turnin-dir/
	rm -rf turnin-dir
}

if [ -n "$1" ]; then
	echo "Beginning the turnin process for $1..."
	turnin-project $1
else
	echo "Please specify which lab you want to turn in."
	echo "Example: ./354turnin.sh lab0"
fi

