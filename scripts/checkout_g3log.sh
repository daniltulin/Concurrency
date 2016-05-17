#!/bin/bash

rm -rf g3log* && \
wget https://github.com/KjellKod/g3log/archive/1.2.zip && \
unzip *.zip && rm -rf *.zip\
mv g3log* g3log
