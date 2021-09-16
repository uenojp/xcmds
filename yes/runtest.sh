#!/bin/bash

gcc -Wall yes.c -o yes

diff <(./yes | head) <(yes | head)
diff <(./yes Hi | head) <(yes Hi | head)
diff <(echo Hi | ./yes | head) <(echo Hi | yes | head)
diff <(echo Hi | ./yes - | head) <(echo Hi | yes - | head)

rm -f ./yes
