#!/bin/bash

cur_dir=$(cd "$(dirname "$0")"; pwd)

# install libcdes.so
# if [ ! -d "$cur_dir/libdes/build" ]; then
#   mkdir $cur_dir/libdes/build
# fi

# cd $cur_dir/libdes/build
# cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
# make
# make install
# ldconfig


# compile libcdesjni.so
if [ ! -d "$cur_dir/ext_jni/build" ]; then
  mkdir $cur_dir/ext_jni/build
fi

cd $cur_dir/ext_jni/build
cmake ..
make 


# compile libcdesjni.so
# export PYTHONMALLOC=malloc
cd $cur_dir/ext_python
python3 setup.py install

