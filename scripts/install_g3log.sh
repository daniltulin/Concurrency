#!/bin/bash
rm -rf g3log_temp
mkdir -p g3log_temp && \
cd g3log_temp && \
wget https://github.com/KjellKod/g3log/archive/1.2.zip && \
unzip *.zip && \
cd g3log* && \
mkdir -p build && cd build && \
cmake ..
