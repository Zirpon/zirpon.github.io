---
title: Obsidian 使用技巧
Author: zirpon
catalog: true
header-img:  "img/header_img/roman.png"
subtitle: The quick brown fox jumps over the lazy dog
categories:
	- quickadd
	- cmenu
	- templater
	- kanban
	- calendar
	- obsidian
	- dataview
	- banner
	- dataviewjs
	- Linter
	- admonition
    - 计算机科学
tags: 
	- diary/year2022
	- diary/month04
	- quickadd
	- cmenu
	- templater
	- kanban
	- calendar
	- obsidian
	- dataview
	- banner
	- dataviewjs
	- Linter
	- admonition

weather: ☀️   🌡️+26°C 🌬️↖4km/h
date: 2022-04-10 12:38:22
update: Sunday-04-10-2022 13:35:52
id: post20220410123822
banner: "_banner/4.gif"
banner_x: 0.5
banner_y: 0.25
top: 999
---

# Obsidian 使用技巧

## 1. quickadd + cmenu +templater + kanban 插件

> 学习资料视频：[obsidian 无障碍联动效率插件 | 手把手打造高效双链个人知识库 | 全主观分享与推荐 | template | quickadd | cmenu](https://www.bilibili.com/video/BV1j34y197cw) 

> 学习视频内嵌模式
<iframe src="//player.bilibili.com/player.html?aid=807319927&bvid=BV1j34y197cw&cid=459209679&page=2" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"> </iframe>

1.  templater 用于设置文章模板
2. quickadd  + cmenu 用于生成 `根据模板新建文章` 的快捷按钮
3. kanban 用于根据 任务 `- [ ]`生成 看板 主要用于日常 todolist
4.  使用 taskList + checklist 追踪 `#check` 任务
5. calendar  + daily note 日记
6. banner 文章顶部背景
7. Linter + YAML 模板头
8. admonition 插件 用做 汇总查询 todolist 的背景部件 具体看 `diaryTemplate` 模板
9. Tasks插件的查询语法详情请看[【效率办公】Obsidain插件之Tasks-任务管理利器](https://www.bilibili.com/read/cv14259903)

## 2. Dataview

### 1. Dataview 查询语法

````text
```dataview
TABLE|LIST|TASK（展现形式）   <field> [AS "显示列名"],..., <field> （显示字段）

FROM <source> (like #tag or "folder")  （查询地址）

WHERE <expression> (like 'field = value')、contains(tags,"ob/ob插件")  （查询条件）

SORT <expression> [ASC/DESC] (like 'field ASC')  （排序）

...other data commands  （其他命令，如limit,group by等）

```
````

学习资料:
- [【效率办公】Obsidain插件之DataView-高效信息动态查询插件](https://zhuanlan.zhihu.com/p/480239685)
- [obsidian插件dataview官方文档翻译](https://zhuanlan.zhihu.com/p/393550306)
- [Step3-知其所以然：一文讲透Obsidian插件DataviewJS](https://www.jianshu.com/p/d1e19530897a)

1. List 查询
	![List查询](https://pic3.zhimg.com/80/v2-15696f4b94599e36caeff3853bf4e95a_720w.jpg)
2. Table 查询
	![](https://pic1.zhimg.com/80/v2-b832f990bcf903ee780a4e045255643c_720w.jpg)
3. Task查询
	![](https://pic3.zhimg.com/80/v2-11243c43b5af8f1381cb9d390896238a_720w.jpg) 
4. Calendar 查询
	![](https://pic3.zhimg.com/80/v2-727b843c979e9259f2b7331795d8d4b2_720w.jpg)
5. DataView 查询字段及命令
	![](https://pic2.zhimg.com/80/v2-51a5b1ea04461d2a55817fc543d79b5d_720w.jpg)
	![](https://pic4.zhimg.com/80/v2-4f1b5823824ac4dd4175e7fd45811a9b_720w.jpg)
	
6. DataView 字段标识
	![](https://pic2.zhimg.com/80/v2-6332b4ff68d06f5b920db268cfe27d91_720w.jpg)

7. 隐含字段

	dataview能自动的对每个页面添加大量的元数据。
	
	-   `file.name`: 该文件标题(字符串)。
	-   `file.folder`: 该文件所在的文件夹的路径(字符串)。
	-   `file.path`: 该文件的完整路径(字符串)。
	-   `file.link`: 该文件的一个链接(链接)。
	-   `file.size`: 该文件的大小(bytes)(数字)
	-   `file.ctime`: 该文件的创建日期(日期和时间)。
	-   `file.cday`: 该文件的创建日期(仅日期)。
	-   `file.mtime`: 该文件最后编辑日期(日期和时间)。
	-   `file.mday`: 该文件最后编辑日期(仅日期)。
	-   `file.tags`: 笔记中所有标签组成的数组。子标签按每个级别进行细分，所以`#Tag/1/A`将会在数组中储存为`[#Tag, #Tag/1, #Tag/1/A]`。
	-   `file.etags`: 笔记中所有显式标签组成的数组；不同于`file.tags`，不包含子标签。
	-   `file.outlinks`: 该文件所有外链(outgoing link)组成的数组。
	-   `file.aliases`: 笔记中所有别名组成的数组。
			如果文件的标题内有一个日期（格式为yyyy-mm-dd或yyyymmdd），或者有一个Date字段/inline字段，它也有以下属性:
	-   `file.day`: 一个该文件的隐含日期。


### 2. Dataviewjs API

1. 原始API
```js
/** A function which maps an array element to some value. */
export type ArrayFunc<T, O> = (elem: T, index: number, arr: T[]) => O;

/** A function which compares two types (plus their indices, if relevant). */
export type ArrayComparator<T> = (a: T, b: T) => number;

/**
 * Proxied interface which allows manipulating array-based data. All functions on a data array produce a NEW array
 * (i.e., the arrays are immutable).
 */
export interface DataArray<T> {
    /** The total number of elements in the array. */
    length: number;

    /** Filter the data array down to just elements which match the given predicate. */
    where(predicate: ArrayFunc<T, boolean>): DataArray<T>;
    /** Alias for 'where' for people who want array semantics. */
    filter(predicate: ArrayFunc<T, boolean>): DataArray<T>;

    /** Map elements in the data array by applying a function to each. */
    map<U>(f: ArrayFunc<T, U>): DataArray<U>;
    /** Map elements in the data array by applying a function to each, then flatten the results to produce a new array. */
    flatMap<U>(f: ArrayFunc<T, U[]>): DataArray<U>;
    /** Mutably change each value in the array, returning the same array which you can further chain off of. */
    mutate(f: ArrayFunc<T, any>): DataArray<any>;

    /** Limit the total number of entries in the array to the given value. */
    limit(count: number): DataArray<T>;
    /**
     * Take a slice of the array. If `start` is undefined, it is assumed to be 0; if `end` is undefined, it is assumbed
     * to be the end of the array.
     */
    slice(start?: number, end?: number): DataArray<T>;
    /** Concatenate the values in this data array with those of another data array. */
    concat(other: DataArray<T>): DataArray<T>;

    /** Return the first index of the given (optionally starting the search) */
    indexOf(element: T, fromIndex?: number): number;
    /** Return the first element that satisfies the given predicate. */
    find(pred: ArrayFunc<T, boolean>): T | undefined;
    /** Find the index of the first element that satisfies the given predicate. Returns -1 if nothing was found. */
    findIndex(pred: ArrayFunc<T, boolean>): number;
    /** Returns true if the array contains the given element, and false otherwise. */
    includes(element: T): boolean;

    /**
     * Return a sorted array sorted by the given key; an optional comparator can be provided, which will
     * be used to compare the keys in leiu of the default dataview comparator.
     */
    sort<U>(key: ArrayFunc<T, U>, direction?: 'asc' | 'desc', comparator?: ArrayComparator<U>): DataArray<T>;

    /**
     * Return an array where elements are grouped by the given key; the resulting array will have objects of the form
     * { key: <key value>, rows: DataArray }.
     */
    groupBy<U>(key: ArrayFunc<T, U>, comparator?: ArrayComparator<U>): DataArray<{ key: U, rows: DataArray<T> }>;

    /**
     * Return distinct entries. If a key is provided, then rows with distinct keys are returned.
     */
    distinct<U>(key?: ArrayFunc<T, U>, comparator?: ArrayComparator<U>): DataArray<T>;

    /** Return true if the predicate is true for all values. */
    every(f: ArrayFunc<T, boolean>): boolean;
    /** Return true if the predicate is true for at least one value. */
    some(f: ArrayFunc<T, boolean>): boolean;
    /** Return true if the predicate is FALSE for all values. */
    none(f: ArrayFunc<T, boolean>): boolean;

    /** Return the first element in the data array. Returns undefined if the array is empty. */
    first(): T;
    /** Return the last element in the data array. Returns undefined if the array is empty. */
    last(): T;

    /** Map every element in this data array to the given key, and then flatten it.*/
    to(key: string): DataArray<any>;
    /**
     * Recursively expand the given key, flattening a tree structure based on the key into a flat array. Useful for handling
     * heirarchical data like tasks with 'subtasks'.
     */
    expand(key: string): DataArray<any>;

    /** Run a lambda on each element in the array. */
    forEach(f: ArrayFunc<T, void>): void;

    /** Convert this to a plain javascript array. */
    array(): T[];

    /** Allow iterating directly over the array. */
    [Symbol.iterator](): Iterator<T>;

    /** Map indexes to values. */
    [index: number]: any;
    /** Automatic flattening of fields. */
    [field: string]: any;
}
```
2. 代码块参考
````text

```dataviewjs
dv.table([], ...)

dv.current()

dv.pages("#books") => all pages with tag 'books'
dv.pages('"folder"') => all pages from folder "folder"
dv.pages("#yes or -#no") => all pages with tag #yes, or which DON'T have tag #no

dv.pagePaths("#books") => the paths of pages with tag 'books'

dv.page("Index") => The page object for /Index
dv.page("books/The Raisin.md") => The page object for /books/The Raisin.md


dv.list([1, 2, 3]) => list of 1, 2, 3
dv.list(dv.pages().file.name) => list of all file names
dv.list(dv.pages().file.link) => list of all file links
dv.list(dv.pages("#book").where(p => p.rating > 7)) => list of all books with rating greater than 


// List all tasks from pages marked '#project'
dv.taskList(dv.pages("#project").file.tasks)

// List all *uncompleted* tasks from pages marked #project
dv.taskList(dv.pages("#project").file.tasks
    .where(t => !t.completed))

// List all tasks tagged with '#tag' from pages marked #project
dv.taskList(dv.pages("#project").file.tasks
	.where(t => t.text.includes("#tag")))

// Render a simple table of book info sorted by rating.
dv.table(["File", "Genre", "Time Read", "Rating"], dv.pages("#book")
    .sort(b => b.rating)
    .map(b => [b.file.link, b.genre, b["time-read"], b.rating]))
    
```
````
3. 查询例子：

````text
```dataviewjs
for (let group of dv.pages("#book").where(p => p["time-read"].year == 2021).groupBy(p => p.genre)) {
 	dv.header(3, group.key);
 	dv.table(["Name", "Time Read", "Rating"],
	 group.rows
		 .sort(k => k.rating, 'desc')
		 .map(k => [k.file.link, k["time-read"], k.rating]))
}
```
````
## 3. [配置文件 | PicGo-Core](https://picgo.github.io/PicGo-Core-Doc/zh/guide/config.html#%E6%89%8B%E5%8A%A8%E7%94%9F%E6%88%90)
