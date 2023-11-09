#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Two arguments are required"
    exit 1
fi

filesdir=".$1"
searchstr=$2

if ! [ -d $filesdir ]; then
    echo "filesdir is not a directory"
    exit 1
fi

matching_lines=`grep -r $searchstr  $filesdir | wc -l`
number_of_files=`find $filesdir -type f | wc -l`

echo "The number of files are $number_of_files and the number of matching lines are $matching_lines" 
exit 0