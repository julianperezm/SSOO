 #!/bin/sh

Error(){
echo "Error">&2
exit 1
}

if test $# -lt 1; then
	Error
elif test -d $1; then
	Error
else
	mkdir $1
	user=$(who| awk '{print $1}');
	for i in $user
	do
		ps -u $i | awk '{print $1}' > $1/$i.log
	done
fi
exit 0
