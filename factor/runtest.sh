#!/bin/bash

gcc -Wall factor.c -o factor

# arg
runtest() {
    diff <(./factor $@) <(factor $@)
}

runtest 99
runtest 0
runtest 1
# runtest -9
# runtest 'foo'
runtest $(seq 10000)

# pipe
runtest() {
    diff <(echo -e $@ | ./factor) <(echo -e $@ | factor)
}

runtest 29
# TODO: allow spaces or tabs
# runtest 57 1729
# runtest $(seq 100)
# runtest '2222 2929\t 92 \t\t \t 99'

# as tmp alt
diff <(seq 1000 | ./factor) <(seq 1000 | factor)

# pipe & arg (input form pipe is ignored)
runtest() {
    diff <(seq $@ | ./factor $@) <(seq $@ | factor $@)
}

runtest 10000

rm -f ./factor
