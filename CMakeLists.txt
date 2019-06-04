cmake_minimum_required(VERSION 2.8)
project(clivia-des)


MESSAGE(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "CMAKE_CURRRENT_BINARY_DIR: ${CMAKE_CURRRENT_BINARY_DIR}")
MESSAGE(STATUS "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}")


add_subdirectory(libdes)
#add_subdirectory(ext_jni)
#add_subdirectory(ext_python)
add_subdirectory(test)
