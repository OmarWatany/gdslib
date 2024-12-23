#!/bin/bash

set -e

files=`echo {arraylist,arraystack,circular_array,linkedlist,pqueue,queue,ringbuffer,stack,tree}`
test_file() {
    echo "Testing $1"
    if [[ -e "./bin/$1" ]]; then
        valgrind -s --track-origins=yes --leak-check=full ./bin/$1 2>&1 
        printf "\n" 
    else
        printf "Error ./bin/$1 doesn't exist\n"
    fi;

    for j in {1..80} ; do printf "*" ; done
    printf "\n" 
}

test () {
  printf "" > $1
  for i in $files ; do
    printf "$i"
    test_file $i | sed "s|==.*==|==[$i]==|"| tee -a $1
  done
}

time test ./test_res
