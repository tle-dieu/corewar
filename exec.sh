#!/bin/bash

i=0
if [[ $(grep -E '\-\-help|\-h' <<< $*) ]]; then
	echo "usage: champ1 champ2 champ3 champ4 vm_option"
else
	for arg in $*; do
		if [[ $i -ge 4 || ${arg:0:1} == '-' ]]; then
			break
		fi
		./asm $arg
		if [[ $? != 0 ]]; then
			echo "ERROR" && exit
		fi
		champ[i]="$(basename "$arg" .s).cor"
		mv -f ${champ[$i]} . 2>/dev/null
		i=$(($i + 1))
	done
	echo "./corewar ${@:$((i + 1))} ${champ[*]}"
	./corewar ${@:$((i + 1))} ${champ[*]}
fi
