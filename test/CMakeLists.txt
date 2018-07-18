cmake_minimum_required(VERSION 2.8)
project(test)


include_directories(
        ${PROJECT_SOURCE_DIR}/../libdes/include
        ${PROJECT_SOURCE_DIR}/
)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -Wall -Wextra -Wno-unused-parameter")
add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64)

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


set (TEST_HEX_SOURCES 
        ${PROJECT_SOURCE_DIR}/tools.c
        ${PROJECT_SOURCE_DIR}/test_hex.c
        )

add_executable(test_hex ${TEST_HEX_SOURCES})
target_link_libraries(test_hex 
    cdes_static
)


set (TEST_HIDE_KEY_SOURCES 
        ${PROJECT_SOURCE_DIR}/tools.c
        ${PROJECT_SOURCE_DIR}/test_hide_key.c
        )

add_executable(test_hide_key ${TEST_HIDE_KEY_SOURCES})
target_link_libraries(test_hide_key 
    cdes_static
)


set (TEST_ENCRYPT_SOURCES 
        ${PROJECT_SOURCE_DIR}/tools.c
        ${PROJECT_SOURCE_DIR}/test_encrypt.c
        )

add_executable(test_encrypt ${TEST_ENCRYPT_SOURCES})
target_link_libraries(test_encrypt 
    cdes_static
)


set (TEST_PERF_SOURCES 
        ${PROJECT_SOURCE_DIR}/tools.c
        ${PROJECT_SOURCE_DIR}/test_perf.c
        )

add_executable(test_perf ${TEST_PERF_SOURCES})
target_link_libraries(test_perf 
    cdes_static
)


set (TEST_FILE_SOURCES 
        ${PROJECT_SOURCE_DIR}/tools.c
        ${PROJECT_SOURCE_DIR}/test_file.c
        )

add_executable(test_file ${TEST_FILE_SOURCES})
target_link_libraries(test_file 
    cdes_static
)
