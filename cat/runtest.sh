#!/bin/bash

gcc -Wall cat.c -o cat

diff <(./cat < /etc/crontab) /etc/crontab
diff <(./cat /etc/crontab /etc/passwd) <(cat /etc/crontab /etc/passwd)
diff <(./cat /etc/crontab /etc/xxxxx 2>&1) <(cat /etc/crontab /etc/xxxxx 2>&1)
diff <(echo foo | ./cat -) <(echo foo | cat -)
diff <(echo foo | ./cat - /etc/crontab) <(echo foo | cat - /etc/crontab)

rm -f ./cat
