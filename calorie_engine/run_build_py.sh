#!/bin/bash

# 切换到脚本文件所在的目录
cd "$( dirname "${BASH_SOURCE[0]}" )"

# 运行 Python 脚本 (在msys2 ucrt64中，需要使用 python3 命令)
python3 build.py "$@"