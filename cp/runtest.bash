#!/bin/bash

cleanup() {
    echo
    rm -rf tmp
    exit 2
}

make > /dev/null || exit 1

mkdir tmp || exit 1

# cp file1 file2
echo -n "cp file file : "
./cp /etc/services tmp/file || cleanup
diff /etc/services tmp/file || cleanup
echo "passed..."

# cp file dir
echo -n "cp file dir : "
./cp /etc/services tmp || cleanup
diff /etc/services tmp/services || cleanup
echo "passed..."

# missing file
echo -n "cp /etc/services : "
result=$(./cp /etc/services 2>&1)
[ "$?" = 1 ] || cleanup
[ "$result" = "cp: missing file" ] || cleanup
echo "passed..."

rm -rf tmp
