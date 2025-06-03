---
title: Hexo blog 详细教程
catalog: true
header-img: "img/header_img/roman.png"
subtitle: The quick brown fox jumps over the lazy dog
date: 2018-12-21 17:43:35
tags:
- hexo
- blog
catagories:
- 计算机科学
top: 99
---

## 1. Hexo 搭建 Blog 基础概念

### 1.1 发布仓库

***`发布仓库`*** 就是日常编写文章以后, 通过命令 `hexo deploy` 发布出去的仓库地址, 具体配置在 `_config.yml` 文件中

```yml
deploy:
    tags: git
    repository: git@ip:repo_name.git
    branch: branch_name
```

- 由于作者本身有云服务器 并且还买了域名 所以我的发布仓库就 建在自己的服务器中 实际上各位客官也可以把仓库建在[gitee](https://gitee.com)的私有仓库 或者 [Github](https://github.com)上
    > 注意：部署在**gitee pages**上要正确填写`_config.yml`的`url`和`root`参数

    - url: https://名字.gitee.io/blog
    - root: /blog
- ***`repo_name`*** 就是仓库的名字了 ***`branch_name`*** 就是仓库的分支名
- 关于这个建仓库 跟 仓库的管理你可能需要一些 ***git*** 的使用知识 具体各位客官自行学习 这里 我可以列举一些可能会使用到的命令

```git
git init --bare repo_name.git
git checkout --track origin branch_name
git pull
git branch -a
git clone repo_name.git
```

- 这个仓库是`hexo deploy` 命令生成的一个静态网页的仓库 所以作者在自己的服务器中 git clone 了这个仓库到一个路径上 然后使用Nginx 配置了该路径的站点 这样就可以访问到这个发布的网站了
- 如果你想要用自己拥有的域名发布网站一般用这个方法比较方便 当然你也可以使用github 提供 `xxx.github.io` 这种方式搭建发布的仓库 不过由于作者本身没有用过 这里的详细步骤请客官自行Google学习
- 这里我贴一下Nginx 的配置吧 这里只是给客官看看配置的例子 由于我配置里面还有很多东西删减 所以 直接拷贝可能无法使用 请客官Google学习以后自行配置 不过应该也差不多的了

```nginx
user nginx;
error_log /var/log/nginx/error.log;
pid /run/nginx.pid;
http {
    server {
        # 博客网站
        listen       80 default_server;
        listen       [::]:80 default_server;
        server_name  _;

        root         /home/xxx/;
        include /etc/nginx/default.d/*.conf;

        location / {
                index  index.php index.html index.htm;
        }
    }
}
```

### 1.2 写作仓库
详细各位客官也是看过 [hexo](https://hexo.io)的官网过来的吧 如果不是也先请你们看看官网的一些介绍比如:
- 环境配置: npm 下之类的 哎 算了 我还是不忍心 让你们跑来跑去的 所以贴一下出来吧 就是安装一下 ***[Node.js](http://nodejs.cn/download/)*** 就好了 至于 node.js 跟 npm 是什么关系这个可以了解一下廖雪峰老师的这篇 ***[文章](https://www.liaoxuefeng.com/wiki/001434446689867b27157e896e74d51a89c25cc8b43bdb3000/00143450141843488beddae2a1044cab5acb5125baf0882000)***
- 初始搭建的命令 相信客官都看了 官网的这个命令 这个命令就是 初始化博客 这个作者称之为 ***`写作仓库`***
    ```shell
    npm install hexo-cli -g
    hexo init blog
    cd blog
    npm install
    hexo server
    ```

- 由于博客都会有主题 每个主题的布置 都不一样 代码不一样 所以 当客官添加评论功能 站长统计功能 添加的 js 代码的地方也不一样
- 作者使用的是 ***[BeanTech](https://github.com/YenYuHsuan/hexo-theme-beantech)*** 的主题
- 所以 作者是 fork 了 BeanTech 的主题然后 在这个仓库里进行 写作的 所以这就是作者所说的 写作仓库
- 在这个仓库中 日常主要执行的命令如下:
    ```shell
    hexo clean
    hexo new article_name
    hexo generate
    hexo deploy
    ```
- 注意这个仓库保存什么文件忽略什么文件 这一点很重要 否则会出现很奇怪的现象:
    - a. 现象: 比如说 你有两台 checkout 了这个仓库的 设备 你把 其中一台A 仓库目录 `.deploy_git` 及其里面的文件都提交了 在另一台设备B 拉取了A 设备提交的 `.deploy_git`,当在B 设备 执行`hexo deploy` 时 会改变当前写作仓库的 `.git` 目录下的 `config` 文件

    - b. 所以 那么写作仓库究竟应该追踪什么文件 跟 忽略什么文件呢 作者为你 列举如下:
        - 跟踪文件:
            ```
            scaffolds/
            source/
            themes/
            _config.yml
            .gitignore
            package.json
            LISCEN
            ```

        - 忽略文件 `.gitignore`:
            ```
            node_modules/
            package-lock.json
            public/
            .deploy_git/
            LICENSE
            db.json
            .deploy_git
            ```

## 2. blog 小功能

### 2.1 增加来必力评论功能

1. 首先你需要到 ***[来必力](https://www.livere.com/login_form)*** 注册账号 得到来必力的 账号id
2. 来必力评论 SNS 可以选择QQ 微博 微信 Github Facebook 注册以后进入设置界面就能看到的了
3. 为BeanTech 添加 来必力 评论 代码
    - themes/beantech/layout/post.ejs

        ```js
        <article>
            <div class="container">
                <div class="row">

                    <!-- Post Container -->
                    <div class="
                        col-lg-8 col-lg-offset-2
                        col-md-10 col-md-offset-1
                        post-container">

                        <%- page.content %>

                        <% if(config['livere_uid']) { %>
                            <!-- disqus 评论框 start -->
                            <div class="comment">
                                <div id="lv-container" data-id="city" data-uid="<%= config['livere_uid'] %>"></div>
                            </div>
                            <!-- disqus 评论框 end -->
                        <% } %>
                    </div>
                </div>
            </div>
        </article>

        <% if(config['livere_uid']) { %>
            <!-- 来必力City版公共JS代码 start (一个网页只需插入一次) -->
            <script type="text/javascript">
            (function(d, s) {
                var j, e = d.getElementsByTagName(s)[0];
                if (typeof LivereTower === 'function') { return; }
                j = d.createElement(s);
                j.src = 'https://cdn-city.livere.com/js/embed.dist.js';
                j.async = true;
                e.parentNode.insertBefore(j, e);
            })(document, 'script');
            </script>
            <noscript>为正常使用来必力评论功能请激活JavaScript</noscript>
            <!-- 来必力City版 公共JS代码 end -->
        <% } %>
        ```

        就是把livere_uid 后面那几行代码 考到 post.ejs 大概这个位置就可以了 主要参考着BeanTech 主题的 disqus 多说评论 代码 就好了

    - themes/beantech/layout/page.ejs

        ```javascript
        <!-- 如果开启评论功能 -->
        <% if(page.comments) { %>
            <hr>
            <% if(config['livere_uid']) { %>
                <!-- disqus 评论框 start -->
                <div class="comment">
                    <div id="lv-container" data-id="city" data-uid="<%= config['livere_uid'] %>"></div>
                </div>
                <!-- disqus 评论框 end -->
            <% } %>

            <% if(config['livere_uid']) { %>
                <!-- 来必力City版公共JS代码 start (一个网页只需插入一次) -->
                <script type="text/javascript">
                    (function(d, s) {
                        var j, e = d.getElementsByTagName(s)[0];
                        if (typeof LivereTower === 'function') { return; }
                        j = d.createElement(s);
                        j.src = 'https://cdn-city.livere.com/js/embed.dist.js';
                        j.async = true;
                        e.parentNode.insertBefore(j, e);
                    })(document, 'script');
                </script>
                <noscript>为正常使用来必力评论功能请激活JavaScript</noscript>
                <!-- 来必力City版 公共JS代码 end -->
            <% } %>
        <% } %>
        ```

### 2.2 增加音乐功能

- 也是修改post.ejs, page.ejs两个文件 感谢 ***[MetingJS](https://github.com/metowolf/MetingJS)***

    ```js
    <!-- require APlayer -->
    <% if(config['metingjs']) { %>
        <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/aplayer@1.10/dist/APlayer.min.css">
        <script src="https://cdn.jsdelivr.net/npm/aplayer@1.10/dist/APlayer.min.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/meting@1.2/dist/Meting.min.js"></script>
        <div class="aplayer"
            data-id="<%= config['data-id'] %>"
            data-server="<%= config['data-server'] %>"
            data-type="<%= config['data-type'] %>"
            data-fixed="<%= config['data-fixed'] %>" >
        </div>
    <% } %>
    ```

### 2.3 增加站点统计功能

- ***[cnzz](https://web.umeng.com/)*** 注册账号 注册以后就拿到一串js 代码 把它 拷贝到 footer.ejs 文件中的 `<footer> </footer>` 就好了
- 有些浏览器可能看不到效果 请换个浏览器试试

## 3. 关于写作

用markdown写作用于生成目录的子标题必须从"**##**"开始, 不能用"#", 这样会导致生成的目录不能跳转

## 4. 后序hexo 博客的一些优化小记录

### hexo-asset-image 并且修改了一点源码以支持本地/在线预览都有效

[本地/在线预览都有效](https://andavid.github.io/2019/01/15/insert-local-image-in-hexo/)

[Hexo插入图片并解决图片的路径问题](https://www.hwpo.top/posts/d87f7e0c/index.html)

### katex 公式问题
[Hexo使用katex渲染公式](https://dora-cmon.github.io/posts/95c5c5d2/)

[Hexo + Butterfly 建站指南（八）使用 KaTeX 数学公式](https://nickxu.me/2022/04/17/Hexo-Butterfly-%E5%BB%BA%E7%AB%99%E6%8C%87%E5%8D%97%EF%BC%88%E5%85%AB%EF%BC%89%E4%BD%BF%E7%94%A8-KaTeX-%E6%95%B0%E5%AD%A6%E5%85%AC%E5%BC%8F/)

[hexo-Butterfly添加数学公式插件功能](https://www.mycloudhub.top/posts/0.html#:~:text=Butterfly%E4%B8%BB%E9%A2%98%E6%94%AF%E6%8C%81Mathjax%E5%92%8CKaTex%E4%B8%A4%E7%A7%8D%E6%95%B0%E5%AD%A6%E5%85%AC%E5%BC%8F%E6%B8%B2%E6%9F%93%E5%BC%95%E6%93%8E%EF%BC%8C%E6%9C%AC%E6%96%87%E9%80%89%E6%8B%A9KaTex%E6%8F%92%E4%BB%B6%EF%BC%8C%E5%9B%A0%E4%B8%BA%20KaTeX%20%E6%9B%B4%E5%BF%AB%E6%9B%B4%E8%BD%BB%E9%87%8F%EF%BC%8C%E5%9B%A0%E6%AD%A4%E6%B2%A1%E6%9C%89%20MathJax%20%E7%9A%84%E5%8A%9F%E8%83%BD%E5%A4%9A%EF%BC%88%E6%AF%94%E5%A6%82%E5%8F%B3%E9%94%AE%E8%8F%9C%E5%8D%95%EF%BC%89%E3%80%82%20%E9%80%89%E6%8B%A9KaTex%20%E4%BD%A0%E4%B8%8D%E9%9C%80%E8%A6%81%E6%B7%BB%E5%8A%A0%20katex.min.js,%E7%9B%B8%E5%BA%94%E7%9A%84%E4%BD%A0%E9%9C%80%E8%A6%81%E5%8D%B8%E8%BD%BD%E4%BD%A0%E4%B9%8B%E5%89%8D%E7%9A%84%20hexo%20%E7%9A%84%20markdown%20%E6%B8%B2%E6%9F%93%E5%99%A8%EF%BC%8C%E7%84%B6%E5%90%8E%E5%AE%89%E8%A3%85%E5%85%B6%E5%AE%83%E6%8F%92%E4%BB%B6%E3%80%82%20%E5%A6%82%E9%9C%80%E9%85%8D%E7%BD%AE%E5%85%B6%E5%AE%83%E5%8F%82%E6%95%B0%EF%BC%8C%E8%AF%B7%E5%8F%82%E8%80%83%20katex%20%E5%AE%98%E7%BD%91)

[解决latex数学公式渲染不正确及行内公式中文渲染乱码问题](https://juejin.cn/post/6968651668720189454)

[Markdown 数学公式排版 KaTex 语法](https://blog.sinlov.cn/posts/2023/03/16/markdown-%E6%95%B0%E5%AD%A6%E5%85%AC%E5%BC%8F%E6%8E%92%E7%89%88-katex-%E8%AF%AD%E6%B3%95/)

[KaTeX 数学公式大全](https://www.luogu.com.cn/article/4a81e2tt)

[Hexo 错误警告与解决办法](https://www.lightshaker.cn/archives/6276/)