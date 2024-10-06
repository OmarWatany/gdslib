#!/bin/sh

set -e

files=`echo {arraylist,arraystack,circular_array,linkedlist,pqueue,queue,ringbuffer,stack,tree}`
test_file() {
    echo 'Testing "$1"'
    valgrind -s --track-origins=yes --leak-check=full ./bin/$1 2>&1
    printf "\n" 
    for j in {1..80} ; do printf "*" ; done
    printf "\n" 
}

test () {
  printf "" > $1
  for i in $files ; do
    test_file $i | sed "s|==.*==|==[$i]==|"| tee -a $1
  done
}

time test ./test_res
