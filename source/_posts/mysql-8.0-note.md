---
title: "MySQL 8.0 note"
catalog: true
date: 2018-11-28 19:43:16
subtitle: "The quick brown fox jumps over the lazy dog"
header-img: "img/header_img/roman.png"
tags:
- mysql
categories:
- 计算机科学
top: 999
---

## mysql 8.0 安装 note

我真的佛了 今天装个mysql 8.0 各种坑 好像不支持 设置set password= password("123456") 这样的写法 各种报错
要

```mysql
mysql> alter user 'root'@'localhost'IDENTIFIED BY 'MyNewPass';

ERROR 1819 (HY000): Your password does notsatisfy the current policy requirements

mysql> alter user 'root'@'localhost'IDENTIFIED BY 'MyNewPass@123';

ERROR 1396 (HY000): Operation ALTER USERfailed for 'root'@'localhost'

mysql> alter user'root'@'%' IDENTIFIED BY 'MyNewPass@123';
```

还有他的密码[安全策略](https://blog.csdn.net/hellosunqi/article/details/70941754)

```mysql
mysql> GRANT REPLICATION CLIENT ON *.*TO 'zabbix'@'%' IDENTIFIED BY ‘xxxxxxxx’;

ERROR 1819 (HY000): Your password does notsatisfy the current policy requirements
```

这个与validate_password_policy的值有关。

validate_password_policy有以下取值：
0 or LOW
Length
1 or MEDIUM
Length; numeric, lowercase/uppercase, and special characters
2 or STRONG
Length; numeric, lowercase/uppercase, and special characters; dictionary file

```mysql
mysql> set global validate_password_length=0；
Query OK, 0 rows affected (0.00 sec)

mysql> SHOW VARIABLES LIKE 'validate_password%';
+--------------------------------------+-------+
| Variable_name                       | Value |
+--------------------------------------+-------+
| validate_password_dictionary_file    |       |
| validate_password_length             | 4    |
| validate_password_mixed_case_count   | 1     |
| validate_password_number_count       | 1     |
| validate_password_policy             | LOW  |
| validate_password_special_char_count | 1     |
+--------------------------------------+-------+
6 rows in set (0.00 sec)
```

还有一个修改存储过程 权限什么的

总之 我佛了

```mysql
# 先创建用户tom，密码为tom
mysql> create user 'tom'@'loaclhost' identified by 'tom';
# 再赋予具体表glodon_test权限
mysql> GRANT ALL privileges ON glodon_test TO 'tom'@'localhost';
Query OK, 0 rows affected (0.08 sec)
# 刷新权限即可生效
mysql> flush privileges;
```