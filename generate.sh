#!/bin/bash
if [[ $# == 0 ]]; then
	echo "Usage: ./generate.sh file.s..."
fi
for ((i = 1; i <= $#; i++ )); do
	if [[ $# && -f ${!i} ]]; then
		res=$(./asm ${!i} --color=f 2>&1)
		if [[ $(echo $res | grep 'Writing output program to') ]]; then
			res=$(./asm -x ${!i} --color=f 2>&1)
		fi
		old=$(sed '/^;/ d' ${!i})
		line=$(grep -m1 '' <<<"$old")
		while [[ -n $old && -z "${line// }" ]]; do
			old=$(sed '1{/^$/d}' <<<"$old")
			line=$(grep -m1 '' <<<"$old")
		done
		sed 's/^/;/' <<<"$res" > ${!i}
		printf "\n%s" "$old" >> ${!i}
		res=$(./asm ${!i} --color=f 2>&1)
		if [[ $(echo $res | grep 'Writing output program to') ]]; then
			res=$(./asm -x ${!i} --color=f 2>&1)
		fi
		old=$(sed '/^;/ d' ${!i})
		sed 's/^/;/' <<<"$res" > ${!i}
		printf "%s" "$old" >> ${!i}
	fi
done
