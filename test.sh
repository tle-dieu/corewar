#!/bin/bash
if [[ $# > 2 ]]; then
	while IFS=  read -r -d $'\0'; do
		echo "${@:2}"
		echo "$REPLY"
	done < <(find $1 -type f -print0)
fi
