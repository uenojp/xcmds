#!/bin/bash

gcc -Wall uniq.c -o uniq

# literal
runtest() {
    diff <(echo -e "$1" | ./uniq) <(echo -e "$1" | uniq)
}

runtest "a\na\na\nx\na\nx\nx"
runtest "abc\nxyz\n123\nx xxxx\nx xxxx\n\tx\n22\n"
runtest ".\n.\n.\n.\n."
runtest "\n"
runtest "\n\n"
runtest ""

# file
runtest() {
    diff <(cat "$1" | ./uniq) <(cat "$1" | uniq)
}

runtest ./data
runtest ./empty

# -
runtest() {
    diff <(echo -e "$1" | ./uniq -) <(echo -e "$1" | uniq -)
}

runtest "a\na\na\nx\na\nx\nx"
runtest "\n"
runtest "\n\n"
runtest ""

rm -f ./uniq
