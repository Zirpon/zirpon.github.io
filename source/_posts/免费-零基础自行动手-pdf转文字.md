---
title: 免费 零基础自行动手 pdf转文字
catalog: true
header-img: img/header_img/roman.png
subtitle: The quick brown fox jumps over the lazy dog
date: 2024-06-03 14:42:19
tags:
- python
- ocrmypdf
- Tesseract
- pdf转文字
catagories:
top: 999
---

# 免费 零基础自行动手 pdf转文字

## 工具

[ocrmypdf 文档](https://ocrmypdf.readthedocs.io/en/latest/index.html)
[ocrmypdf 文档 官网具体下载教程](https://ocrmypdf.readthedocs.io/en/latest/installation.html)
[Ghostscript 下载地址](https://ghostscript.com/releases/gsdnld.html)

- Python 64-bit
- Tesseract 64-bit
- Ghostscript 64-bit

```bat
winget install -e --id Python.Python.3.11
winget install -e --id UB-Mannheim.TesseractOCR
python3 -m pip install ocrmypdf
```

## 当前目录 批量转化pdf为可复制文字的图片 执行代码

[代码文件](batch.py)

```sh
python ./batch.py '.'
```

### batch.py 源码
```py
#!/usr/bin/env python3
from __future__ import annotations

import logging
import sys
from pathlib import Path

import ocrmypdf

# pylint: disable=logging-format-interpolation
# pylint: disable=logging-not-lazy

script_dir = Path(__file__).parent
print(script_dir)

if len(sys.argv) > 1:
    start_dir = Path(sys.argv[1])
else:
    start_dir = Path('.')

print(start_dir)

if len(sys.argv) > 2:
    log_file = Path(sys.argv[2])
else:
    log_file = script_dir.with_name('ocr-tree.log')

print(log_file)

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s %(message)s',
    filename=log_file,
    filemode='a',
)

ocrmypdf.configure_logging(ocrmypdf.Verbosity.quiet)

for filename in start_dir.glob("**/*.pdf"):
    logging.info(f"Processing {filename}")
    print(f"Processing {filename}")
    print(filename)
    output_file = f"{filename}_tmp.pdf"
    print(output_file)

    result = ocrmypdf.ocr(filename, output_file, deskew=True, language=["chi_sim","eng"], jobs=2, skip_text=True, progress_bar=True, optimize=1, png_quality=30, jpeg_quality=40)
    #, jbig2_page_group_size=1

    if result == ocrmypdf.ExitCode.already_done_ocr:
        logging.error("Skipped document because it already contained text")
    elif result == ocrmypdf.ExitCode.ok:
        logging.info("OCR complete")
    logging.info(result)
```

## 汇总

### OCR识别pdf

> ocrmypdf --force-ocr old.pdf new.pdf

### 配合img2pdf，把图片转为OCR识别的pdf

> img2pdf --pagesize A4 page*.png | ocrmypdf - myfile.pdf

### 如果使用 poppler 得安装MSCV 
- [ ] [教程](https://blog.csdn.net/sinat_37967865/article/details/102477235)
- [ ] [ocr2text 教程](https://github.com/writecrow/ocr2text)
### 把OCR识别转为txt文本

[英文教程](https://github.com/writecrow/ocr2text)

[超详细解决pytesseract.pytesseract.TesseractNotFoundError: tesseract is not installed or it‘s not in yo...](https://blog.csdn.net/m0_37576542/article/details/132315537)

[pymupdf 文档](https://pymupdf.readthedocs.io/en/latest/pixmap.html#Pixmap.pil_tobytes)

[tesseract-ocr 使用文档](https://tesseract-ocr.github.io/tessdoc/Command-Line-Usage.html)
例如：
> tesseract.exe images/eurotext.png -l eng+deu

- [ ] 下载 [anaconda](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)
- [ ] 下载 [tesseract-ocr](https://github.com/UB-Mannheim/tesseract/wiki) 安装程序 ***记得勾选中文支持库***
- [ ] pip install pytesseract 
    1. pytesseract 实际上是使用python代码 执行 `tesseract.exe` 程序 所以传参方式还是参考 `tesseract-ocr 使用文档`
    2. 另外还需要 修改 pytesseract 库中 `tesseract.exe` 的执行路径 为用户安装 `tesseract-ocr` 的路径
    3. 另外 把`tesseract.exe`的安装路径 以及 `tesseract-ocr` 文字库的路径 设置于系统变量
    
    具体参考 [超详细解决pytesseract.pytesseract.TesseractNotFoundError: tesseract is not installed or it‘s not in yo...](https://blog.csdn.net/m0_37576542/article/details/132315537)

- [ ] 下载 识别转化脚本
    > git clone https://github.com/writecrow/ocr2text.git

- [ ] 运行 `Anaconda Powershell Prompt` 然后 cd `ocr2text` 目录下 执行
    > pip install --user --requirement requirements.txt 
    > python3.12.exe .\ocr2text.py

### ocr2text.py 源码 带点小修改

```py
import os
import shutil
import errno
import subprocess
import tempfile
from tempfile import mkdtemp
import re
import io

try:
    from PIL import Image
except ImportError:
    print('Error: You need to install the "Image" package. Type the following:')
    print('pip install Image')

try:
    import pytesseract
except ImportError:
    print('Error: You need to install the "pytesseract" package. Type the following:')
    print('pip install pytesseract')
    exit()

try:
    from pdf2image import convert_from_path, convert_from_bytes
except ImportError:
    print('Error: You need to install the "pdf2image" package. Type the following:')
    print('pip install pdf2image')
    exit()

import time
import sys
from textract import exceptions


def update_progress(progress):
    barLength = 10  # Modify this to change the length of the progress bar
    status = ""
    if isinstance(progress, int):
        progress = float(progress)
    if not isinstance(progress, float):
        progress = 0
        status = "error: progress var must be float\r\n"
    if progress < 0:
        progress = 0
        status = "Halt...\r\n"
    if progress >= 1:
        progress = 1
        status = "\r\n"
    block = int(round(barLength*progress))
    text = "\rPercent: [{0}] {1}% {2}".format(
        "#"*block + "-"*(barLength-block), progress*100, status)
    sys.stdout.write(text)
    sys.stdout.flush()
    print()


def run(args):
        # run a subprocess and put the stdout and stderr on the pipe object
        try:
            pipe = subprocess.Popen(
                args,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            )
        except OSError as e:
            if e.errno == errno.ENOENT:
                # File not found.
                # This is equivalent to getting exitcode 127 from sh
                raise exceptions.ShellError(
                    ' '.join(args), 127, '', '',
                )

        # pipe.wait() ends up hanging on large files. using
        # pipe.communicate appears to avoid this issue
        stdout, stderr = pipe.communicate()

        # if pipe is busted, raise an error (unlike Fabric)
        if pipe.returncode != 0:
            raise exceptions.ShellError(
                ' '.join(args), pipe.returncode, stdout, stderr,
            )

        return stdout, stderr


def extract_tesseract(filename):
        #temp_dir = mkdtemp()
        #base = os.path.join(temp_dir, 'conv')
        #temp_dir = 'tempdir'           
        temp_dir, _ = os.path.splitext(filename)
        
        if not os.path.exists(temp_dir):
                os.makedirs(temp_dir)
        contents = []
        try:
        
            import pymupdf

            doc = pymupdf.open(filename) # open a document

            for page_index in range(len(doc)): # iterate over pdf pages
                page = doc[page_index] # get the page
                image_list = page.get_images()

                # print the number of images found on the page
                #if image_list:
                #    print(f"Found {len(image_list)} images on page {page_index}")
                #else:
                #   print("No images found on page", page_index)

                for image_index, img in enumerate(image_list, start=1): # enumerate the image list
                    xref = img[0] # get the XREF of the image
                    pix = pymupdf.Pixmap(doc, xref) # create a Pixmap

                    if pix.n - pix.alpha > 3: # CMYK: convert to RGB first
                        pix = pymupdf.Pixmap(pymupdf.csRGB, pix)

                    pix.save("%s\\page_%05d-image_%02d.png" % (temp_dir, page_index, image_index)) # save the image as png
                    png_bytes = pix.tobytes()
                    page_content = pytesseract.image_to_string(Image.open(io.BytesIO(png_bytes)), 'eng+chi_sim')
                    contents.append(page_content)
                    pix = None

            for dirpath, dirnames, files in os.walk(temp_dir):
                for name in files:
                    filename, file_extension = os.path.splitext(name)
                    if (file_extension.lower() != '.png'):
                        continue
                    source_path = os.path.join(dirpath, name)
                    relative_directory = os.path.dirname(os.path.realpath(name))
                    pngfile = os.path.join(relative_directory, source_path)
                    #print('pic path: ' + repr(pngfile))

                    #page_content = pytesseract.image_to_string(Image.open(io.BytesIO(pngfile)), 'eng+chi_sim')
                    #contents.append(page_content)
            
            return ''.join(contents)

            # https://python-pptx.readthedocs.io/en/latest/
            #stdout, _ = run(['pdftoppm', filename, base])
            print('extract_tesseract: ' + filename)
            for page in sorted(os.listdir(temp_dir)):
                page_path = os.path.join(temp_dir, page)
                # https://github.com/Belval/pdf2image
                page_content = pytesseract.image_to_string(Image.open(page_path))
                contents.append(page_content)
            return ''.join(contents)
        finally:
            #shutil.rmtree(temp_dir)
            pass


def convert_recursive(source, destination, count):
    pdfCounter = 0
    for dirpath, dirnames, files in os.walk(source):
        for file in files:
            if file.lower().endswith('.pdf'):
                pdfCounter += 1
    print('pdfCounter: ' + str(pdfCounter))
    ''' Helper function for looping through files recursively '''
    for dirpath, dirnames, files in os.walk(source):
        for name in files:
            filename, file_extension = os.path.splitext(name)
            if (file_extension.lower() != '.pdf'):
                continue
            print('name:' + name)
            relative_directory = os.path.relpath(dirpath, source)
            source_path = os.path.join(dirpath, name)
            output_directory = os.path.join(destination, relative_directory)
            output_filename = os.path.join(output_directory, filename + '.txt')
            if not os.path.exists(output_directory):
                os.makedirs(output_directory)
            count = convert(source_path, output_filename, count, pdfCounter)
    return count


def convert(sourcefile, destination_file, count, pdfCounter):
    text = extract_tesseract(sourcefile)
    with open(destination_file, 'w', encoding='utf-8') as f_out:
        f_out.write(text)
    print()
    print('Converted ' + source)
    count += 1
    update_progress(count / pdfCounter)
    return count

count = 0
print()
print('********************************')
print('*** PDF to TXT file, via OCR ***')
print('********************************')
print()
dir_path = os.path.dirname(os.path.realpath(__file__))
print('Source file or folder of PDF(s) [' + dir_path + ']:')
print('(Press [Enter] for current working directory)')
source = input()
if source == '':
    source = dir_path

print('Destination folder for TXT [' + dir_path + ']:')
print('(Press [Enter] for current working directory)')
destination = input()
if destination == '':
    destination = dir_path

if (os.path.exists(source)):
    if (os.path.isdir(source)):
        count = convert_recursive(source, destination, count)
    elif os.path.isfile(source):  
        filepath, fullfile = os.path.split(source)
        filename, file_extension = os.path.splitext(fullfile)
        if (file_extension.lower() == '.pdf'):
            count = convert(source, os.path.join(destination, filename + '.txt'), count, 1)
    plural = 's'
    if count == 1:
        plural = ''
    print(str(count) + ' file' + plural + ' converted')
else:
    print('The path ' + source + ' seems to be invalid')

```
## 然后就能通过朗读 边读边看 提升阅读效率

## [pdf转docx](https://pdf2docx.readthedocs.io/en/latest/index.html)