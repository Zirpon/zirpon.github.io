---
title: javascript 笔记
catalog: true
header-img: img/header_img/roman.png
subtitle: The quick brown fox jumps over the lazy dog
top: 9
date: 2024-07-06 23:39:44
tags:
catagories:
---

# javascript 笔记

## basic

[HTML5+JavaScript实现语音合成（文字转语音）](https://blog.csdn.net/cnds123/article/details/137920674)
[JS实现将文字转换为语音并自动播放](https://segmentfault.com/a/1190000041989692)
[JavaScript中的一次性定时器和周期性定时器](https://cloud.tencent.com/developer/article/1797513)
[简易开关](https://blog.csdn.net/qq_41675812/article/details/131554034)

```js
//	var a='xixi'; 
	if(typeof(a) == "undefined" || a == null)
		alert("a is undefined");
	else
		alert("a is defined");
```
typeof运算符的返回类型为字符串，值包括如下几种：

'undefined' --未定义的变量或值

'boolean' --布尔类型的变量或值

'string' --字符串类型的变量或值

'number' --数字类型的变量或值

'object' --对象类型的变量或值，或者null(这个是js历史遗留问题，将null作为object类型处理)

'function' --函数类型的变量或值


[HTML/CSS switch开关 （包括JS控制checked选择）](https://www.cnblogs.com/it-Ren/p/13062999.html)

## [Tampermonkey中文文档](https://bbs.tampermonkey.net.cn/forum.php?mod=viewthread&tid=1909)

- [世界通用版本号规则](https://semver.org/lang/zh-CN/)
- [menu](https://bbs.tampermonkey.net.cn/forum.php?mod=viewthread&tid=271)
- [Webpack+TypeScript 油猴脚本开发模板，支持本地开发，在线热刷新，Tampermonkey template，油猴开发框架，以斗鱼直播自动切换清晰度作为示例的油猴脚本开发模板](https://github.com/Eished/tampermonkey-template)
- [油猴脚本tampermonkey脚手架，完全支持TypeScript、热更新部署](https://github.com/xcr1234/tampermonkey-typescript)

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