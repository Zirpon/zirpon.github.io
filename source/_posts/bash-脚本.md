---
title: bash 脚本
catalog: true
header-img: img/header_img/roman.png
subtitle: The quick brown fox jumps over the lazy dog
top: 999999
date: 2024-07-07 04:44:12
tags:
- bash
catagories: 
- 计算机科学
---

# bash 脚本

## basic

```
history  5
grep -10 'qsm' test.log//打印匹配行的前后10行
```

## 分割大文本文件

```bash
  wc -l all.log
  split -l 35000 -d --verbose all.log all-log-split-
  for i in `ls | grep all-log-split-`;do a=`echo $i.txt`; mv $i $a;done
```

Here is a footnote reference,[^1] and another.[^longnote]

## Endnotes

[^1]: Here is the footnote.
[^longnote]: Here's one with multiple blocks.

[label]: <https://> "website title"
