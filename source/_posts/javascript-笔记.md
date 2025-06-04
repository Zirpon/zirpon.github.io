---
title: javascript 笔记
catalog: true
header-img: img/header_img/roman.png
subtitle: The quick brown fox jumps over the lazy dog
top: 99991910
date: 2024-07-06 23:39:44
tags:
    - javascript
    - 文字转语音
    - threejs
    - 3D
    - sketchfab
categories:
- 计算机科学
---

# javascript 笔记

## basic

[JavaScript 中的一次性定时器和周期性定时器](https://cloud.tencent.com/developer/article/1797513)
[JavaScript ES6-10 语法](https://iknow.fun/2020/06/02/javascript-es6-10/#1-1-%E8%AF%BE%E7%A8%8B%E5%AF%BC%E5%AD%A6-%E8%AF%95%E7%9C%8B)
[HTML/CSS switch 开关 （包括 JS 控制 checked 选择）](https://www.cnblogs.com/it-Ren/p/13062999.html)
[JavaScript 字典遍历用法介绍](https://geek-docs.com/javascript/javascript-ask-answer/102_tk_1703987553.html)
[JS 字典遍历](https://geek-docs.com/javascript/javascript-ask-answer/55_hk_1709422196.html)
[非阻塞弹窗](https://sweetalert.js.org/guides/)

```js
for (const [key, value] of Object.entries(object)) {
	console.log(key, value);
}

var points = [40, 100, 1, 5, 25, 10];
points.sort(function (a, b) {
	return a - b;
});

//	var a='xixi';  function  object number string boolean undefined
if (typeof a == "undefined" || a == null) alert("a is undefined");
else alert("a is defined");

myString = "129";
console.log(parseInt(myString)); // expected result: 129

Number("10.33"); // returns 10.33

console.log(+""); // expected output: 0

parseFloat("10 20 30"); // returns 10
parseFloat("10 years"); // returns 10
parseFloat("years 10"); // returns NaN

str = "1222";
console.log(Math.floor(str)); // returns 1222

str = "2344";
console.log(str * 1); // expected result: 2344

negStr = "-234";
console.log(~~negStr); // expected result: -234

Array.from({length: 10}, (val, i) => i) // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
Array.from(new Array(10).keys()) // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
Object.keys(Array.from({length: 10})).map((val, i ) => +i) // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
[...new Array(10).keys()]; // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
[...Array.from({length: 10}).keys()] // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
```

### [js中(function(){}()) (function(){})() $(function(){});之间的区别](https://blog.csdn.net/stpice/article/details/80586444)

```js
//这两种写法，都是一种立即执行函数的写法，即IIFE (Immediately Invoked Function Expression)。这种函数在函数定义的地方就直接执行了。
(function(){}())

(function(){})()

//是$(document).ready(function(){/*...*/})的简写形式，是在DOM加载完成后执行的回调函数，并且只会执行一次。
$(function(){/*...*/})

```

## html

```html
<div id="foo\bar"></div>
<div id="foo:bar"></div>

<script>
  console.log("#foo\bar"); // "#fooar"
  document.querySelector("#foo\bar"); // 不匹配任何元素

  console.log("#foo\\bar"); // "#foo\bar"
  console.log("#foo\\\\bar"); // "#foo\\bar"
  document.querySelector("#foo\\\\bar"); // 匹配第一个 div

  document.querySelector("#foo:bar"); // 不匹配任何元素
  document.querySelector("#foo\\:bar"); // 匹配第二个 div
</script>
```

```js
var el = document.querySelector(".myclass");

var el = document.querySelector("div.user-panel.main input[name='login']");
```

## [bun][]

-   windows install

```bash
bun --revision
powershell -c "irm bun.sh/install.ps1|iex"
bun add -d @types/bun
bun add -d @types/react@latest @types/react-dom@latest
bun add -d @babel/core @babel/preset-react
bun add -d @types/figlet

bun --watch run dev

#`foo, bar and baz`
bun run --filter 'ba*' <script>

echo "console.log('Hello')" | bun run -

bun --smol run index.tsx

bun --print process.env
bun --print process.env.production
bun --print process.env.development
bun --print process.env.local

bun build ./cli.ts --compile --outfile mycli
# cli.ts
console.log("Hello world!");

bun build --compile --target=bun-windows-x64 ./path/to/my/app.ts --outfile myapp

bun build --compile --target=bun-linux-x64 ./index.ts --outfile myapp

bun build --compile --target=bun-darwin-x64 ./path/to/my/app.ts --outfile myapp

bun build --compile --minify --sourcemap ./path/to/my/app.ts --outfile myapp

```

### 弹窗插件 

- [sweetalert2](https://sweetalert2.github.io/#examples)
- [弹窗队列案列](https://sweetalert2.github.io/recipe-gallery/queue-with-progress-steps.html)

#### 弹窗队列 

- 自己实现的 开箱即用的弹窗队列 源码 [AlertQueue][]


### jsx

-   bun add -d @types/react@latest

```jsx
import "react";

function Component(props: { message: string }) {
	return (
		<body>
			<h1 style={{ color: "red" }}>{props.message}</h1>
		</body>
	);
}

console.log(<Component message="Hello world!" />);
```

```ts
import db from "./my.db" with { tags: "sqlite" };
console.log(db.query("select * from users LIMIT 1").get());

import myEmbeddedDb from "./my.db" with { tags: "sqlite", embed: "true" };
console.log(myEmbeddedDb.query("select * from users LIMIT 1").get());

// this becomes an internal file path
import icon from "./icon.png" with { tags: "file" };
import { file } from "bun";

export default {
  fetch(req) {
    // Embedded files can be streamed from Response objects
    return new Response(file(icon));
  },
};

import icon from "./icon.png" with { tags: "file" };
import { file } from "bun";
const bytes = await file(icon).arrayBuffer();

import { stuff } from "./my-commonjs.cjs";
import Stuff from "./my-commonjs.cjs";
const myStuff = require("./my-commonjs.cjs");

```

### [plugin](https://bun.sh/docs/runtime/plugins) 没看太懂

## Javascrip 变声 文字转语音

https://github.com/w-okada/voice-changer
AI 语音开源模型下载地址：https://discord.gg/aihub

[HTML5+JavaScript 实现语音合成（文字转语音）](https://blog.csdn.net/cnds123/article/details/137920674)
[JS 实现将文字转换为语音并自动播放](https://segmentfault.com/a/1190000041989692)
[最新 rvc 实时变声（附模型、安装、教程地址）voice-changer，AI 唱歌](https://www.bilibili.com/video/BV11F41197AP/?spm_id_from=333.880.my_history.page.click&vd_source=b48342a630f5cc1a5c86649a37c0db89)
[js 版本变声器](https://github.com/Venryx/w-okada-voice-changer-scripts/tree/master)

## javascript 3D 渲染

-   [sketchfab_doc][]
-   [epic_sketchfab][]
-   [dragon][]
-   sketchfab
-   [threejs_doc][]

Here is a footnote reference,[^1] and another.[^longnote]

## Endnotes

[^1]: Here is the footnote.
[^longnote]: Here's one with multiple blocks.

[bun]: https://bun.sh/docs/installation
[sketchfab_doc]: https://sketchfab.com/developers/data-api/v3 "website title"
[epic_sketchfab]: https://support.fab.com/s/article/Embedding-your-3D-models
[dragon]: https://sketchfab.com/3d-models/old-god-elder-dragon-be305f220c404e28afa073ddbc62873d "Old God Elder Dragon"
[threejs_doc]: http://webgl3d.cn/pages/aac9ab/
[AlertQueue]: <https://raw.githubusercontent.com/Zirpon/zirpon.github.io/master/source/src/AlertQueue.js>
