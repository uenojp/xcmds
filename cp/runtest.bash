#!/bin/bash

make > /dev/null || exit 1

mkdir -p tmp || exit 1
./cp /etc/services tmp/file
diff /etc/services tmp/file

rm -rf tmp
