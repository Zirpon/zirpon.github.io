# git

## git command

- git init 生成 .git 文件 => 版本库 repository 仓库
- git add readme.txt
- git commit -m "wrote a readme file"
- git status
- git diff
- git log --pretty=online
- git reset --hard HEAD^
                   HEAD^^
                   HEAD~100
                   版本号
- git relog
- git checkout --file 文件返回到最近一次 git commit 或 git add 状态
- git rm
- git remote add
- git push


> git config --global user.name "Your Name"
> git config --global user.email "xxx@example.com"

## 标签管理

- git tag v1.0
- git log --pretty=online --abbrev-commit
- git tag v0.9
- git show v0.9
- git tag-a v0.1 -m "version 0.1 realeased" 3628164
- git tag -d v1.0
- git tag -d v0.9
- git push origin:ref/tags/v0.9

## 分支管理

- git checkout -b dev
- git branch
- git add readme.txt
- git commit -m "branch test"
- git checkout master
- git merge dev
- git branch -d dev
- git merge --no-ff -m "merge with no-ff" dev
- git stash
    ```
    git stash
    git checkout master
    git checkout -b issue-101
    git chekcout master
    git merge --no-ff -m "~~" issue-101
    git branch -d issue-101
    git checkout dev
    git stash list
    git stash pop/git stash apply stash@{0}
    ```
- git checkout -b dev origin/dev
- git push origin dev
- git pull
- git branch --set-upstream dev origin/dev
- git pull

- 交换

a=a^b
b=b^a
a=a^b
