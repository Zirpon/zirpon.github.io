find . -name TowerDefenseAward.txt | xargs grep "50[0:1]60[1-7]" | awk -F '\t' '{split($1,timer,":");i
f(timer[1] == 10 && timer[2] >= 30){print $0;} else if (timer[1] == 11 && timer[2] <= 45){print $0;}}'

find . -name TowerDefenseAward.txt | xargs grep "50[0:1]60[1-7]" | awk -F '\t' '{split($1,timer,":");if(($10 >= 500602 && $10 <= 500607 && $6 = 10)||($10 >= 501601 && $10 <= 501606 && $6 = 10)){if(timer[1] == 10 && timer[2] >= 30){print $0;} else if (timer[1] == 11 && timer[2] <= 45){print $0;}}}'

find ./*/2015-10-13/ -name TowerDefenseAward.txt | xargs grep "50[0:1]60[1-7]" | awk -F '\t' '{split($1,timer,":");if(($10 >= 500602 && $10 <= 500607 && $6 = 10)||($10 >= 501601 && $10 <= 501606 && $6 = 10)){if(timer[2] == 10 && timer[3] >= 30){print $0;} else if (timer[2] == 11 && timer[3] <= 45){print $0;}}}' > TowerDefenseAward_copy.txt

find ./*/2015-10-13/ -name TowerDefenseAward.txt | xargs grep "50[0:1]60[1-7]" | awk -F '' '{split($1,timer,":");if(($10 >= 500602 && $10 <= 500607 && $6 = 10)||($10 >= 501601 && $10 <= 501606 && $6 = 10)){if(timer[2] == 10 && timer[3] >= 30){print $0;} else if (timer[2] == 11 && timer[3] <= 45){print $0;}}}' > TowerDefenseAward_copy.txt