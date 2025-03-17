#!/bin/sh
#格斗之皇, 大陆服务器,登陆脚本
#/web/gdzh_vxinyou*/paymaent/
#/web/gdzh_vxinyou_com/payment/
#payCallback.php pay.php
#scp test.php root@113.107.205.185:/web/gdzh_vxinyou_com/payment/test.php
#scp root@113.107.205.185:/web/gdzh_vxinyou_com/payment/test.php /Users/champon/Desktop/test.php 远程到本地
#continent 没有开 sshd, 不能使用 scp 传输 continent文件到本地, 只能在本地 scp continent 的文件
#/var/tmp
#/var/log

# 参数传输有误
if [ $# -lt 2 ]; then
echo "param error"
exit
fi

file=$1
dir=$2

echo $file
echo $dir
date

#set timeout 1800
if [ "$1" == "none" ]; then
    scp ./* root@113.107.205.185:/web/gdzh_vxinyou_com/channel_sdk/demo/$2
else
    scp $1 root@113.107.205.185:/web/gdzh_vxinyou_com/channel_sdk/Sdk/Driver/$1
fi

#scp channel_dianwan.class.php root@113.107.205.185:/web/gdzh_vxinyou_com/channel_sdk/Sdk/Driver/channel_dianwan.class.php

#scp root@113.107.205.185:/web/gdzh_vxinyou_com/payment/test.php /Users/champon/Desktop/test.php 远程到本地

#expect "password:"
#send "root@mO~dGk-sD*pfgOiGK-TA_3tDD*1eDUm8\r"
#expect eof
#EOF

echo "scp finished"
exit 0