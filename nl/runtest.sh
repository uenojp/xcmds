#!/bin/bash

gcc nl.c -o nl

# literal
runtest() {
	diff <(echo -e "$1" | ./nl) <(echo -e "$1" | nl)
}

runtest 'abc\ndef\ng'
runtest 'abc\ndef'
runtest 'abc\n\ndef'
runtest 'abc\n\n\ndef'
runtest 'abc\n'
runtest '\n'
runtest '\n\n'
runtest ''


# file
runtest() {
	for f in "$@";do
		files="$files"" ""$f"
	done
	diff <(./nl $files) <(nl $files)
}

runtest /etc/shells /etc/crontab
# over buffer size
runtest /etc/services /etc/lvm/lvm.conf


# file & stdin
runtest() {
	diff <(echo -e "$1" | ./nl -) <(echo -e "$1" | nl -)
	diff <(echo -e "$1" | ./nl - "$2") <(echo -e "$1" | nl - "$2")
}

runtest 'abc\ndef\ng' /etc/shells
runtest 'abc\n' 	  /etc/shells
runtest '\n'          /etc/shells
runtest '\n\n'        /etc/shells
runtest '' 			  /etc/shells

rm -f ./nl
