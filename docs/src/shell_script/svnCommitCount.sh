
if [[ $# < 2 ]]; then
    echo "too few agr"
fi

username=$1

for var in "$@"
do
    if [[ $var = $username ]]; then
        continue
    fi

    # echo svn log --search $username --search-and $var -q
    svn log --search "$username" --search-and "$var" -q >> repositoryList.txt 
done

# cat repositoryList.txt

vs=
while read line
do
    arr=($line)
    vs="$vs "${arr[0]}
done < repositoryList.txt
# echo $vs

marr=($vs)
num=${#marr[*]}
echo "there are ${num} versions"

for ((i=0;i<num;i++))
{
    szVersion=${marr[i]}
    if [[ $szVersion = "------------------------------------------------------------------------" ]]; then
        continue;
    fi
   	len_szVersion=${#szVersion}
   	nVersion=${szVersion:1:${len_szVersion}}

    let first=$nVersion
    let second=${first}-1

    # echo $first
    # echo $second

    svn diff -r ${first}:${second} >> diff.txt
}


let n=0
while read line
do
	if [[ ${line:0:1} = "+" ]];then
	let n++
	fi

    if [[ ${line:0:3} = "+++" ]];then
    let n--
    fi
	# if [[ ${line:0:1} = "-" ]];then
	# let n++
	# fi
	
done < diff.txt

echo $n

  rm diff.txt
  rm repositoryList.txt
