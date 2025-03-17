find . -name DungeonSweeping.txt | xargs grep "50[1:2:3]4[1:2][0-9]" | awk -F '  ' '{size=$7/100;size=int(size);if(size == 5014 || size =
= 5024 || size == 5034) {k=$7%100;if(k >= 10 && k <= 20);print $0}}'


find . -name DungeonSweeping.txt | xargs grep "50[1:2:3]4[1:2][0-9]" | awk -F '\t' '{size=$7/100;size=int(size);if(size == 5014 || size == 5024 || size == 5034) {k=$7%100;if(k >= 10 && k <= 20) {split($1,k_time,":");if(k_time[1] == 10 && k_time[2] >= 30){print $0} else if(k_time[1] == 11 && k_time[2] <= 45) {print $0};}}}'

find ./*/2015-10-13/ -name DungeonSweeping.txt | xargs grep "50[1:2:3]4[1:2][0-9]" | awk -F '\t' '{size=$7/100;size=int(size);if(size == 5014 || size == 5024 || size == 5034) {k=$7%100;if(k >= 10 && k <= 20) {split($1,k_time,":");if(k_time[2] == 10 && k_time[3] >= 30){print $0} else if(k_time[2] == 11 && k_time[3] <= 45) {print $0};}}}' > DungeonSweeping_copy.txt

find ./*/2015-10-13/ -name FinishDunge.txt | xargs grep "" | awk -F '' '{{if($6 >= 1 && $6 <= 3) {split($1,k_time,":");if(k_time[2] == 10 && k_time[3] >= 30){print $0} else if(k_time[2] == 11 && k_time[3] <= 45) {print $0};}}}' > FinishDunge_copy.txt