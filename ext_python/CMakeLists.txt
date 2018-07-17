cmake_minimum_required(VERSION 3.0)
project(ext_python)

set(Python_ADDITIONAL_VERSIONS 3.6.5)
find_package(PythonLibs)
include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../libdes/include
)

set(SOURCES
        ${PROJECT_SOURCE_DIR}/src/encrypt_facade.c
        )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Wextra -Wno-unused-parameter")
add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64)
#add_definitions(-DDEBUG_FLAG)

if (APPLE)
    add_definitions(-D_DARWIN_USE_64_BIT_INODE=1 -D_DARWIN_UNLIMITED_SELECT=1)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_BUILE_TYPE DEBUG)
    set(CMAKE_C_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
else()
    add_definitions(-D_GNU_SOURCE -D_POSIX_C_SOURCE=200112)
    add_definitions(-Wstrict-aliasing)
    add_definitions(-std=c99)
    add_definitions(-fPIC)
	add_definitions(-g)
    add_definitions(-O3)
endif()

find_path(CDES_HEADER NAMES cdes/encrypt.h PATHS /usr/local/include)
find_library(CDES_LIB NAMES cdes PATHS /usr/local/lib)

add_library(cdespyt SHARED ${SOURCES})
