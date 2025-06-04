---
title: 小说 《A不在现场》爬虫
catalog: true
header-img: img/header_img/roman.png
subtitle: The quick brown fox jumps over the lazy dog
date: 2024-05-19 22:33:13
tags:
- 爬虫
- scrapy
categories: 
- 计算机科学
top: 999
---

# scrapy 爬虫

## 爬取 小说正文 http://st.kanxshuo.com

保存为markdown 格式 [《A不在现场》前11章](kanxshuo-他不在现场-苏格拉夫顿-27846.txt)

### 使用 vim 正则表达式 处理连接修改 视频字幕小说正文

[《不在现场》11章到最后一章](《不在现场》11章到最后一章.txt)

### 源码

- [spider-kanxshuo.py](spider-kanxshuo.py) 
- [mylog.py](./utils/mylog.py)
- 依赖 
  - [scrapy](https://docs.scrapy.org/en/latest/) 
  - [中文版](https://scrapy-16.readthedocs.io/zh-cn/)

#### spider-kanxshuo.py

> 其中 `./utils` 文件夹 与 `spider-kanxshuo.py` 同级

```python
import scrapy
from scrapy.selector import Selector
import json

from utils import mylog
logger = mylog.mylogger(__name__)

outDict = []
bookCode = '27846'
website = 'st.kanxshuo.com/'
class kanxshuoSpider(scrapy.Spider):
    name = 'kanxshuo-'+str(bookCode)
    start_urls = [f'http://{website}/book-{bookCode}-1.html',]

    def parse(self, response):
        global outDict

        urltitle = response.css('div.bm_h::text').get()       
        
       #windows 文件名不能把包含以下字符
        urltitle = urltitle.replace('\\', '').replace('/', '').replace(':', '').replace('*', '')\
        .replace('?', '').replace('"', '').replace('<', '').replace('>', '').replace('|', '').replace('<br>', '')
        
        body = response.css('div.bookContent').get()
        body = body.replace('</div>', '').replace('<br>', '\n')\
            .replace('<div class="bookContent" id="fontzoom">', '')\
            .replace('<div id="a_d_4"><script type="text/javascript" src="/skin/a_728.js"></script>','')\
            .replace('<span id="a_d_1"> <script type="text/javascript" src="/skin/a_336.js"></script> </span>', '')\
            .replace('<span id="a_d_2"> <script type="text/javascript" src="/skin/a_728.js"></script> </span>', '')
        logger.info('当前页: {}'.format(urltitle))
        logger.debug('正文: {}'.format(body))
        outDict.append({
            '当前页': urltitle,
            '正文': body,
        })

        yield {
            '当前页': urltitle,
            '正文': body,
        }

        next_page = None
        for nextpage in response.css('div.bpages').css('a'):
            pagename = nextpage.css('a.pn::text').get()
            #logger.debug('{} '.format(pagename))
            if pagename == '下─页':
                next_page = nextpage.css('a::attr("href")').get()
                logger.debug(next_page)
                break
            #elif pagename != '尾页' and pagename != '首页' and pagename != '上一页' \
            #and int(pagename) and int(pagename) > 5000000:
            #    break
        if next_page is not None:
            yield response.follow(next_page, self.parse)
        else:
            outjsonName = "./txt-jsons/kanxshuo-%s-%s.json" % (urltitle, bookCode)
            #logger.debug("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
            logger.debug(outDict)
            outMDName = "./txt-markdown/kanxshuo-%s-%s.md" % (urltitle, bookCode)
            with open(outMDName, 'w+', newline='', encoding='utf-8') as ff:
                for item in outDict:
                    title =  item['当前页']
                    body = item['正文']
                    md_item = f'##### {title}' + '\n'
                    md_item += f'{body}' + '\n'
                    md_item += '\n---\n\n'
                    logger.info(md_item)
                    ff.writelines(md_item)
            #with open(outjsonName, "w+", encoding='utf-8') as f:
            #    # json.dump(dict_, f)  # 写为一行
            #    json.dump(outDict, f, indent=2, sort_keys=False, ensure_ascii=False)  # 写为多行
```

#### ./utils/mylog.py

```python
import logging, coloredlogs

def mylogger(name):
    logger = logging.getLogger(name)

    level_styles = coloredlogs.DEFAULT_LEVEL_STYLES.copy()
    level_styles['debug'] = {'color': 'magenta'}
    level_styles['info'] = {'color': 'yellow'}
    level_styles['error'] = {'color': 'red'}
    level_styles['warning'] = {'color': 'blue'}
    coloredlogs.install(
        level="DEBUG",  # show only debug and above
        #fmt="%(asctime)s - %(hostname)s - %(name)s[%(process)d] -\
        #  %(pathname)s -%(filename)s - %(funcName)s - %(lineno)d - %(module)s - %(levelname)s - %(message)s",
        fmt="%(asctime)s - %(hostname)s - %(name)s[%(process)d] - %(filename)s::%(funcName)s::%(lineno)d - %(levelname)s - %(message)s",
 
        logger=logger,
        level_styles=level_styles,
    )
    return logger

    """
        logger.debug('Print log level：debug')
        logger.info('Print log level：info')
        logger.warning('Print log level：warning')
        logger.error('Print log level：error')
        logger.critical('Print log level：critical')
    """
```

### python 执行脚本

`scrapy runspider spider-kanxshuo.py -o spider-kanxshuo.json -s FEED_EXPORT_ENCODING=UTF-8 -s LOG_FILE=spider-kanxshuo.log`

## 爬取 B站 《庆余年》 有声小说视频 (视频+小说=伴读书郎)

- 下载B站 有声小说视频 使用剪映 识别视频字幕 保存为小说正文
- 使用 vim 正则表达式 处理 爬取小说正文
- [《庆余年》小说正文](《庆余年》_qinkan.net.txt)

## 爬虫本地数据库 NoSQL数据库

- LiteDB
    - [litedb_doc][]
    - [LiteDB][]

- [FileXdb.py][FileXdb]

- TinyDB
    - [和SQLite数据库对应的NoSQL数据库：TinyDB的详细使用（python3经典编程案例][]
    - [Python TinyDB库：轻量级NoSQL数据库的终极指南][]
    - [tinydb_doc][]

### TinyDB

#### 更快的json读写插件 [BetterJSONStorage][]

```python
from BetterJSONStorage import BetterJSONStorage
from tinydb import TinyDB, Query
from pathlib import Path

path = Path('./output/moviedb4.json')
moviedb = TinyDB(path, access_mode="r+", storage=BetterJSONStorage)

moviedb.default_table_name = 'movietable'
movietable = moviedb.table('movietable')

query = Query()

# 插入或者更新DB
ret = movietable.upsert(movieJson, query.name == name)

# 查询DB
if movietable.get(query.name == name) is None:
    pass

doc_id = movietable.get(query.name == name).doc_id

# 更新DB
movietable.update({'url':url}, doc_ids = [doc_id])
movietable.update({'screenshots':jsonResult['screenshots']}, doc_ids = [doc_id])
movietable.update({'cover':cover_url}, doc_ids = [doc_id])
logger.debug(movietable.get(doc_id=doc_id))

# 打印DB
def print_db():
    outMDName = "./db/actorDB.md"
    logger.info('outMDName total: {}'.format(outMDName))

    # 取数据写 markdown
    with open(outMDName, 'w+', newline='', encoding='utf-8') as ff:
        logger.info('outMDName: {}'.format(outMDName))
        with TinyDB(path, access_mode="r+", storage=BetterJSONStorage) as actorDB:
            logger.info('DB total: {}'.format(actorDB))
            actorTable = actorDB.table('actorTable')
            logger.info('DB table: {}'.format(actorTable))

            i = 0
            for item in actorTable.all():
                logger.info(item)

                md_item = print_item(item)
                i+=1

                ff.writelines(md_item)
                ff.flush()
                logger.info(md_item)
                logger.info(i)
            actorDB.close()
        ff.close()
    return

# 写入DB
def flush_db():
    outDict = []
    ...
    outDict.append(user_dict)
    ...
    path = Path(f'./db/actorDB.json')
    with TinyDB(path, access_mode="r+", storage=BetterJSONStorage) as actorDB:
        actorDB.default_table_name = 'actorTable'
        actorTable = actorDB.table('actorTable')
        query = Query()
        for actor in outDict:
            logger.info('actor:::::::::::{}'.format(actor))
            actorTable.upsert(actor, query.name == actor.name'])
        logger.info('DB total: {}'.format(actorDB))
        actorDB.close()
```

Here is a footnote reference,[^1] and another.[^longnote]

## Endnotes
[^1]: Here is the footnote.
[^longnote]: Here's one with multiple blocks.

[BetterJSONStorage]: <https://github.com/MrPigss/BetterJSONStorage>
[FileXdb]: <https://github.com/FileXdb/FileXdb.py>
[LiteDB]: <https://github.com/lidanger/LiteDB.wiki_Translation_zh-cn?tab=readme-ov-file>
[litedb_doc]: <https://dev.listera.top/docs/litedb/>
[和SQLite数据库对应的NoSQL数据库：TinyDB的详细使用（python3经典编程案例]: https://blog.csdn.net/cui_yonghua/article/details/120060474
[tinydb_doc]: <https://tinydb.readthedocs.io/en/latest/index.html>
[Python TinyDB库：轻量级NoSQL数据库的终极指南]: <https://blog.csdn.net/GitHub_miao/article/details/139188174>