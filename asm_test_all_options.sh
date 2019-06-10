#!/bin/bash

PROGRAM="asm"
OPTIONS=""
FILE_NAME="*"
LOG_PATH="error/fsanitize.log"

export ASAN_OPTIONS="log_path=$LOG_PATH color=always"
rm -rf error
mkdir error
old=0
printf "0%%"
percent=$(echo "$(find $* -type f -name "$FILE_NAME" | wc -l) * 8 + 7" | bc)
i=0
while IFS= read -r -d $'\0'; do
	./$PROGRAM -x $REPLY > /dev/null 2>&1 
	nb_file=$(ls -1 error/ | wc -l)
	if [[ $nb_file -gt $old ]]; then
		new_file=$(ls error/ -Art | tail -n 1)
		echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
		old=$nb_file
	fi
	i=$((i + 1))
	nb=$(echo "$i * 100 / $percent" | bc)
	printf "\033[2K\r$nb%%"

	./$PROGRAM -xl $REPLY > /dev/null 2>&1 
	nb_file=$(ls -1 error/ | wc -l)
	if [[ $nb_file -gt $old ]]; then
		new_file=$(ls error/ -Art | tail -n 1)
		echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
		old=$nb_file
	fi
	i=$((i + 1))
	nb=$(echo "$i * 100 / $percent" | bc)
	printf "\033[2K\r$nb%%"

	./$PROGRAM -xl $REPLY > /dev/null 2>&1 
	nb_file=$(ls -1 error/ | wc -l)
	if [[ $nb_file -gt $old ]]; then
		new_file=$(ls error/ -Art | tail -n 1)
		echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
		old=$nb_file
	fi
	i=$((i + 1))
	nb=$(echo "$i * 100 / $percent" | bc)
	printf "\033[2K\r$nb%%"

	./$PROGRAM -b $REPLY > /dev/null 2>&1 
	nb_file=$(ls -1 error/ | wc -l)
	if [[ $nb_file -gt $old ]]; then
		new_file=$(ls error/ -Art | tail -n 1)
		echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
		old=$nb_file
	fi
	i=$((i + 1))
	nb=$(echo "$i * 100 / $percent" | bc)
	printf "\033[2K\r$nb%%"

	./$PROGRAM -bl $REPLY > /dev/null 2>&1 
	nb_file=$(ls -1 error/ | wc -l)
	if [[ $nb_file -gt $old ]]; then
		new_file=$(ls error/ -Art | tail -n 1)
		echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
		old=$nb_file
	fi
	i=$((i + 1))
	nb=$(echo "$i * 100 / $percent" | bc)
	printf "\033[2K\r$nb%%"

	./$PROGRAM -o test.cor $REPLY > /dev/null 2>&1 
	nb_file=$(ls -1 error/ | wc -l)
	if [[ $nb_file -gt $old ]]; then
		new_file=$(ls error/ -Art | tail -n 1)
		echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
		old=$nb_file
	fi
	i=$((i + 1))
	nb=$(echo "$i * 100 / $percent" | bc)
	printf "\033[2K\r$nb%%"
	rm -f test.cor

	./$PROGRAM -d $REPLY > /dev/null 2>&1 
	nb_file=$(ls -1 error/ | wc -l)
	if [[ $nb_file -gt $old ]]; then
		new_file=$(ls error/ -Art | tail -n 1)
		echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
		old=$nb_file
	fi
	i=$((i + 1))
	nb=$(echo "$i * 100 / $percent" | bc)
	printf "\033[2K\r$nb%%"

	./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1 
	nb_file=$(ls -1 error/ | wc -l)
	if [[ $nb_file -gt $old ]]; then
		new_file=$(ls error/ -Art | tail -n 1)
		echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
		old=$nb_file
	fi
	i=$((i + 1))
	nb=$(echo "$i * 100 / $percent" | bc)
	printf "\033[2K\r$nb%%"

done < <(find $* -type f -name "$FILE_NAME" -print0)
./$PROGRAM -color=a $REPLY > /dev/null 2>&1 
nb_file=$(ls -1 error/ | wc -l)
if [[ $nb_file -gt $old ]]; then
	new_file=$(ls error/ -Art | tail -n 1)
	echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
	old=$nb_file
fi
i=$((i + 1))
nb=$(echo "$i * 100 / $percent" | bc)
printf "\033[2K\r$nb%%"

./$PROGRAM -color=f $REPLY > /dev/null 2>&1 
nb_file=$(ls -1 error/ | wc -l)
if [[ $nb_file -gt $old ]]; then
	new_file=$(ls error/ -Art | tail -n 1)
	echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
	old=$nb_file
fi
i=$((i + 1))
nb=$(echo "$i * 100 / $percent" | bc)
printf "\033[2K\r$nb%%"

./$PROGRAM -color=forcea $REPLY > /dev/null 2>&1 
nb_file=$(ls -1 error/ | wc -l)
if [[ $nb_file -gt $old ]]; then
	new_file=$(ls error/ -Art | tail -n 1)
	echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
	old=$nb_file
fi
i=$((i + 1))
nb=$(echo "$i * 100 / $percent" | bc)
printf "\033[2K\r$nb%%"

./$PROGRAM -color=al $REPLY > /dev/null 2>&1 
nb_file=$(ls -1 error/ | wc -l)
if [[ $nb_file -gt $old ]]; then
	new_file=$(ls error/ -Art | tail -n 1)
	echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
	old=$nb_file
fi
i=$((i + 1))
nb=$(echo "$i * 100 / $percent" | bc)
printf "\033[2K\r$nb%%"

./$PROGRAM -color=n $REPLY > /dev/null 2>&1 
nb_file=$(ls -1 error/ | wc -l)
if [[ $nb_file -gt $old ]]; then
	new_file=$(ls error/ -Art | tail -n 1)
	echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
	old=$nb_file
fi
i=$((i + 1))
nb=$(echo "$i * 100 / $percent" | bc)
printf "\033[2K\r$nb%%"

./$PROGRAM -color=autom $REPLY > /dev/null 2>&1 
nb_file=$(ls -1 error/ | wc -l)
if [[ $nb_file -gt $old ]]; then
	new_file=$(ls error/ -Art | tail -n 1)
	echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
	old=$nb_file
fi
i=$((i + 1))
nb=$(echo "$i * 100 / $percent" | bc)
printf "\033[2K\r$nb%%"

./$PROGRAM -color=aut $REPLY > /dev/null 2>&1 
nb_file=$(ls -1 error/ | wc -l)
if [[ $nb_file -gt $old ]]; then
	new_file=$(ls error/ -Art | tail -n 1)
	echo "./$PROGRAM $OPTIONS $REPLY > /dev/null 2>&1" | cat - error/$new_file > temp && mv temp error/$new_file
	old=$nb_file
fi
i=$((i + 1))
nb=$(echo "$i * 100 / $percent" | bc)
printf "\033[2K\r$nb%%"
echo
