---
title: javascript 笔记
catalog: true
header-img: img/header_img/roman.png
subtitle: The quick brown fox jumps over the lazy dog
top: 9
date: 2024-07-06 23:39:44
tags:
- javascript
- 文字转语音
- threejs
- 3D
- sketchfab
catagories:
---

# javascript 笔记

## basic

[JavaScript中的一次性定时器和周期性定时器](https://cloud.tencent.com/developer/article/1797513)
[JavaScript ES6-10语法](https://iknow.fun/2020/06/02/javascript-es6-10/#1-1-%E8%AF%BE%E7%A8%8B%E5%AF%BC%E5%AD%A6-%E8%AF%95%E7%9C%8B)
[HTML/CSS switch开关 （包括JS控制checked选择）](https://www.cnblogs.com/it-Ren/p/13062999.html)
[JavaScript字典遍历用法介绍](https://geek-docs.com/javascript/javascript-ask-answer/102_tk_1703987553.html)
[JS 字典遍历](https://geek-docs.com/javascript/javascript-ask-answer/55_hk_1709422196.html)

```js
for (const [key, value] of Object.entries(object)) {
    console.log(key, value);
}

var points = [40,100,1,5,25,10];
points.sort(function(a,b){return a-b});

//	var a='xixi';  function  object number string boolean undefined
	if(typeof(a) == "undefined" || a == null)
		alert("a is undefined");
	else
		alert("a is defined");
```

## Javascrip 变声 文字转语音

https://github.com/w-okada/voice-changer
AI语音开源模型下载地址：https://discord.gg/aihub

[HTML5+JavaScript实现语音合成（文字转语音）](https://blog.csdn.net/cnds123/article/details/137920674)
[JS实现将文字转换为语音并自动播放](https://segmentfault.com/a/1190000041989692)
[最新rvc实时变声（附模型、安装、教程地址）voice-changer，AI唱歌](https://www.bilibili.com/video/BV11F41197AP/?spm_id_from=333.880.my_history.page.click&vd_source=b48342a630f5cc1a5c86649a37c0db89)
[js版本变声器](https://github.com/Venryx/w-okada-voice-changer-scripts/tree/master)

## javascript 3D 渲染

- [sketchfab_doc][]
- [epic_sketchfab][]
- [dragon][]
- sketchfab
- [threejs_doc][]

Here is a footnote reference,[^1] and another.[^longnote]

## Endnotes

[^1]: Here is the footnote.
[^longnote]: Here's one with multiple blocks.

[sketchfab_doc]: <https://sketchfab.com/developers/data-api/v3> "website title"
[epic_sketchfab]: <https://support.fab.com/s/article/Embedding-your-3D-models>
[dragon]: <https://sketchfab.com/3d-models/old-god-elder-dragon-be305f220c404e28afa073ddbc62873d> "Old God Elder Dragon"
[threejs_doc]: <http://webgl3d.cn/pages/aac9ab/>