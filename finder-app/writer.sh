#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Two arguments are required"
    exit 1
fi

writefile=$1
writestr=$2

IFS='/' read -a path_array <<< "$writefile" 

path_array_len=${#path_array[@]}

for ((i=0; i<$path_array_len; i++))
do
    if [ $i -gt 0 ]; then
        if [ $i -eq `expr $path_array_len - 1` ]; then
            touch ${path_array[$i]}
            echo $writestr > ${path_array[$i]}
        else
            mkdir -p ${path_array[$i]}
            cd ${path_array[$i]}
        fi
    fi
done

exit 0