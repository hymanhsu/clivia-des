cmake_minimum_required(VERSION 2.8)
project(ext_jni)

find_package(JNI REQUIRED)
include_directories(${JAVA_INCLUDE_PATH})
include_directories(${JAVA_INCLUDE_PATH2})

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/../libdes/include
)

set(SOURCES
        ${PROJECT_SOURCE_DIR}/../libdes/src/hex.c
        ${PROJECT_SOURCE_DIR}/../libdes/src/des.c
        ${PROJECT_SOURCE_DIR}/../libdes/src/encrypt.c
        ${PROJECT_SOURCE_DIR}/../libdes/src/helper.c
        ${PROJECT_SOURCE_DIR}/src/com_sogou_nlu_clivia_commons_encrypt_EncryptFacade.c
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

add_library(cdesjni SHARED ${SOURCES})
