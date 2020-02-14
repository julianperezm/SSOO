#!/bin/sh

error(){
echo "Wrong arguments" >&2
exit 1
}

listDir() {
	files=$(find $1)
    for i in $files; do
		if file $i | egrep -q 'ASCII text'; then
			bytes=$(ls -l $i | awk '{print $5}')
			if test $bytes -ge $size; then
				echo $bytes " " $i
			fi
		fi
    done
}

if test $# -eq 1; then
	size=0
	listDir $1 | sort -rn
elif test $# -eq 2; then
	size=$1
	listDir $2 | sort -rn
else
	error
fi
exit 0
