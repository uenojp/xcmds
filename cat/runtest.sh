#!/bin/bash

make > /dev/null || exit 1

test1() {
    output1=$(./cat $1 $2 2>&1); status1="$?"
    output2=$(  cat $1 $2 2>&1); status2="$?"
    [ "$output1" = "$output2" ] || {
            echo "output: expected $output1, but got. $output2"
            exit 2
        }
    [ "$status1" = "$status2" ] || {
            echo "status: expected $status1, but got. $status2"
            exit 3
        }
}

test1 "/etc/crontab"
test1 "/etc/crontab" "-n"
test1 "/etc/crontab /etc/passwd"
test1 "/etc/crontab /etc/passwd" "-n"
test1 "/xxx/yyy"
# test1 "" "-x"             # Non-existent option
# test1 "/etc/crontab" "-x" # Non-existent option

diff <(echo foo | ./cat -) <(echo foo | cat -)
diff <(echo foo | ./cat - /etc/crontab) <(echo foo | cat - /etc/crontab)
