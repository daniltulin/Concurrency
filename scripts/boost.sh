#!/bin/bash
wget http://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.gz?r=&ts=1459461116&use_mirror=nchc 
#https://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz/download
mv download boost.tar.gz
gzip -d boost.tar.gz
tar xvf boost.tar > /dev/null
cd boost_1_60_0/
echo $CPLUS_INCLUDE_PATH
