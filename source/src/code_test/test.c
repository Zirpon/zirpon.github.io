# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

// 读不会修改文件偏移量 写会修改文件偏移量 O_APPEND每次写都会从文件底部开始写
int main()
{    
    char buf[50];

    int fd = openat(AT_FDCWD, "booktest.txt", O_RDWR|O_APPEND|O_EXCL|O_TRUNC);
    
    write(fd, "abcdefg", 7);
    int offset = lseek(fd, 0, SEEK_CUR);
    printf("\nbuf:%s,%d,%d,%s,%d\n",buf, offset,errno,strerror(errno),fd);
    
    offset = lseek(fd, 0, SEEK_SET);
    read(fd, buf, 3);
    printf("\nbuf:%s,%d,%d,%s,%d\n",buf, offset,errno,strerror(errno),fd);
    
    write(fd, "44", 2);
    offset = lseek(fd, 0, SEEK_CUR);
    printf("\nbuf:%s,%d,%d,%s,%d\n",buf, offset,errno,strerror(errno),fd);

    close(fd);
    exit(0);
}