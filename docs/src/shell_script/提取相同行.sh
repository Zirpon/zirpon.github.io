
arr=
let n=1

while read line
do
	arr[$n]=$line
	let n++
done < "log_0831.txt"

arr_roleid_time_key=
let k=1
for ((i = 1; i < ${#arr[*]}; i++))
do
	row=${arr[$i]}

	arr_row=($row)
	roleid_time_key=${arr_row[1]}

	arr_roleid_time_key[$k]=$roleid_time_key
	let k++
done

same_row_index=
let h=1
for ((i = 1; i < ${#arr_roleid_time_key[*]}; i++))
do
	let should_skip=0
	for (( x = 1; x < ${#same_row_index[*]}; x++ )); do
		if [[ $i -eq ${same_row_index[$x]} ]]; then
			let should_skip++
			break
		fi
	done
	if [[ $should_skip -eq 1 ]]; then
		continue
	fi

 	key=${arr_roleid_time_key[$i]}

 	local match_key=
 	let g=1
	for ((j = 1; j < ${#arr_roleid_time_key[*]}; j++))
	do
	if [ ${arr_roleid_time_key[$j]} = $key ]; then
			match_key[$g]=$j
			let g++

			same_row_index[$h]=$j
			let h++
	fi
 	done

 	# echo $i ${#match_key[*]}
 	if [[ ${#match_key[*]} -eq 1 ]]; then
 		continue
 	fi

 	for (( j = 1; j <= ${#match_key[*]}; j++ )); do
 		index=${match_key[$j]}
 		# echo $index
 		str_row=${arr[$index]}
 		echo $str_row >> tmp_filter.txt
 	done
done