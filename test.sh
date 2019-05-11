#!/bin/bash
if [[ $# > 2 ]]; then
	while IFS=  read -r -d $'\0'; do
		echo "${@:3}"
		echo "$REPLY"
	done < <(find $1 -name "$2" -print0)
fi
