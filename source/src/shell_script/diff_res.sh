#!/bin/sh
#对比更新包res.zip和线上正在运行的res
#列出有修改的配置
cd $(dirname $0)
[ -e res.zip ] && rm -f res.zip
[ -d res ] && rm -rf res
wget http://192.168.1.17:10101/res.zip
unzip -q res.zip

echo  '对比更新包res.zip和线上正在运行的res'
rsync -rvccn res/zh_tw/ /soft/zh_tw/srv_1101/GameServer/res/zh_tw/ | sed '1d' | tac | sed '1,3d' | tac > changed_res_list.txt
if [ $(cat changed_res_list.txt |wc -l) != 0 ]
then
	echo "对比资源更新包，以下文件有修改:"
	awk -F/ '{print $NF}' changed_res_list.txt | sort -t'.' -k2 | nl
else
	echo '没有发现差异，资源配置为最新。'
fi
