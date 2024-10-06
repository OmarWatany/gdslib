#!/bin/sh

set -e

test_file() {
    echo "Testing $1"
    valgrind -s --track-origins=yes --leak-check=full $1 2>&1
    printf "\n" 
    for j in {1..80} ; do printf "*" ; done
    printf "\n" 
}

test () {
  printf "" > $1
  for i in ./bin/* ; do
    test_file $i | tee -a $1
  done
}

time test ./test_res
