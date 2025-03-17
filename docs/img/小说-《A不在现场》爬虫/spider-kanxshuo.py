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


