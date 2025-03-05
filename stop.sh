pid=`lsof -i:4000 | grep hexo | awk -F' ' '{print $2}'`
kill -9 $pid
