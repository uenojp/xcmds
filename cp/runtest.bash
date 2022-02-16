#!/bin/bash

cleanup() {
    echo
    rm -rf tmp
    exit 2
}

make > /dev/null || exit 1

# cp file1 file2
mkdir tmp || exit 1
echo -n "cp file1 file2 : "
./cp /etc/services tmp/file || cleanup
diff /etc/services tmp/file || cleanup
echo "passed..."
rm -rf tmp

# cp file dir/file
mkdir tmp || exit 1
echo -n "cp file dir/file : "
./cp /etc/services tmp/s || cleanup
diff /etc/services tmp/s || cleanup
echo "passed..."
rm -rf tmp

# cp file dir/
mkdir tmp || exit 1
echo -n "cp file dir : "
./cp /etc/services tmp/ || cleanup
diff /etc/services tmp/services || cleanup
echo "passed..."
rm -rf tmp

# cp file1 file2... dir/
mkdir tmp || exit 1
echo -n "cp file dir : "
./cp /etc/services /etc/crontab tmp/ || cleanup
diff /etc/services tmp/services || cleanup
diff /etc/crontab tmp/crontab|| cleanup
echo "passed..."
rm -rf tmp

# missing file
mkdir tmp || exit 1
echo -n "(error) cp file : "
result=$(./cp /etc/services 2>&1)
[ "$?" = 1 ] || cleanup
[ "$result" = "cp: missing file" ] || cleanup
echo "passed..."
rm -rf tmp

# not a regular file
mkdir tmp || exit 1
echo -n "(error) cp character_file : "
result=$(./cp /dev/stdin tmp/foo 2>&1)
[ "$?" = 1 ] || cleanup
[ "$result" = "cp: /dev/stdin is not a regular file" ] || cleanup
echo "passed..."
rm -rf tmp

# not a directory
mkdir tmp || exit 1
echo -n "(error) cp file1 file2 file3 : "
touch tmp/file{1,2,3}
result=$(./cp file1 file2 file3 2>&1)
[ "$?" = 1 ] || cleanup
[ "$result" = "cp: file3 is not a directory" ] || cleanup
echo "passed..."
rm -rf tmp
