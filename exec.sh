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
		mv -f "$(echo "$arg" | rev | cut -d. -f2- | rev).cor" .
		champ[i]="$(basename "$arg" .s).cor"
		i=$(($i + 1))
	done
	echo "./corewar ${@:$((i + 1))} ${champ[*]}"
	./corewar ${@:$((i + 1))} ${champ[*]}
fi
