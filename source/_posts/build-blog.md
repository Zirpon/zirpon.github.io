---
title: "Build Blog"
catalog: true
date: 2018-11-18 19:43:16
subtitle: "Build 到 怀疑人生"
header-img: "img/header_img/roman.png"
tags:
- build blog
categories:
- 琐事
top: 9
---

今天花了一天的时间建博客 心很累 昨天就开始研究的了 但是昨天一直卡在一个有 js bug的 hexo 主题上面 [Hux](https://github.com/Huxpro/huxpro.github.io) 各种留 issue求教 没反应 研究不下来 溜了溜了

今天早上稍微看了一下昨天的问题 还是不行 决定放弃 转投其他比如 [BeanTech](ttps://github.com/YenYuHsuan/hexo-theme-beantech.git) 直接可用 但还是心累

后来我就开始改东西啦 各种网站名 作者乱七八糟的配置 遇到改主题 这个卡了好久 最后还是放弃 是什么故事呢 且听我娓娓道来 哎 但是我现在好饿 一天只吃了早餐 到现在 差不多晚上八点了... 我的天折腾了一天

好了 是什么呢 就是用 nginx 建个文件服务器 用来给 markdown存放 resource 图片之类的引用 然后给他建了个目录 作为跳转下载 然后 就是 php 变量名 . 跟 _ 的转换 正则表达式 一切准备就绪 然而 填上主题背景的获取 ip 后 发现并不能获取展现出来 不懂 js 所以不知道为什么 哎 倒不如说不想去看 js 代码吧 然后直接放弃 文件服务器 直接把自己的资源放在工程里面了 这样 别人都拿到我的照片了啊啊啊啊 心累 不过先暂时这样吧 没心情捣鼓了

心累二 接着 发现 图片显示不美观 然后又各种截图 直到美观为止...
心累三 找 js 代码里面 Beantech 写死的网址 配置什么的 然后 改 也累 我是对前端代码非常抗拒 真的 太~~~~~臃肿了
心累四 _config.yml里面关于作者介绍的字符串变量`sidebar-about-description` 中间加了`<br>` 并没有换行 然后对比 BeanTech 的参考源 Hux 的代码 发现并不一样啊 然后 [Hux](https://github.com/Huxpro/huxpro.github.io) 的能换行 BeanTech 的不行

Hux 的源文件short-about.ejs

```js
    <p><%= config['sidebar-about-description'] %></p>
```

Hux 的源文件short-about.html

```html
    <p>{{site.sidebar-about-description}}</p>
```

是的 我有试过把 Hux 的拷过来 明显不行 声明类的方式 读取配置的方式 不一样吧 感觉 哎 反正不懂 反正很烦 好吧 但我还是稍微查了一下 js 的语法 想看看 有没有什么方法能够让`<%= %>` 它读到的字符串 能够识别这个换行符 然而找不到啊  炸了 放弃了
我不行了 我太饿了 再见
