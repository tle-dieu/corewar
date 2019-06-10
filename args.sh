#!/bin/bash

PROGRAM="asm"
OPTIONS=""
FILE_NAME="*"
LOG_PATH="error/fsanitize.log"

export ASAN_OPTIONS="log_path=$LOG_PATH color=always"
rm -rf error
mkdir error
old=0
while IFS= read -r -d $'\0'; do
	./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1 
	nb_file=$(ls -1 error/ | wc -l)
	if [[ $nb_file -gt $old ]]; then
		new_file=$(ls error/ -Art | tail -n 1)
		echo $REPLY | cat - error/$new_file > temp && mv temp error/$new_file
		old=$nb_file
	fi
done < <(find $* -type f -name "$FILE_NAME" -print0)
