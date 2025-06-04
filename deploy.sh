hexo clean && rm -f db.json && hexo g --silent
rm -rf ./docs
cp -rf ./public ./docs
