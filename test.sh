#!/bin/bash

if [[ "$1" == "corewar" ]]; then
	./asm resources/champs/basics/zork.s > /dev/null 2>&1
	mv resources/champs/basics/zork.cor . > /dev/null 2>&1
	if [[ $# > 1 && -f "zork.cor" ]]; then
		while IFS=  read -r -d $'\0'; do
			echo "./corewar zork.cor $REPLY"
			valgrind ./corewar zork.cor $REPLY
		done < <(find $2 -type f -name "*.cor" -print0)
		rm -f zork.cor
	else
		echo "missing args"
	fi
elif [[ "$1" == "asm" ]]; then
	if [[ $# > 2 ]]; then
		while IFS=  read -r -d $'\0'; do
			echo "${@:3}"
			echo "$REPLY"
		done < <(find $2 -type f -print0)
	else
		echo "missing args"
	fi
fi
