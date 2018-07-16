#!/usr/bin/env python3
# encoding: utf-8

from distutils.core import setup, Extension

pcdes_module = Extension('pcdes', 
      sources = [
            '../libdes/src/des.c',
            '../libdes/src/encrypt.c',
            '../libdes/src/helper.c',
            '../libdes/src/hex.c',
            'src/encrypt_facade.c'
      ],
      include_dirs = [
            '../libdes/include'
            ],
      extra_compile_args=[
            '-O3','-std=c99','-fPIC','-g',
#            '-DDEBUG_FLAG',
            '-DUSED_BY_PYTHON',
            '-D_LARGEFILE_SOURCE',
            '-D_FILE_OFFSET_BITS=64']
)

setup(name='pcdes',
      version='0.1.0',
      description='pcdes module written in C',
      ext_modules=[pcdes_module])
