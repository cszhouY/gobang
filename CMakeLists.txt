cmake_minimum_required (VERSION 3.25)
project (gobang)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_BUILD_TYPE "Debug")

aux_source_directory(./src DIR_SRCS)

add_executable(gobang ${DIR_SRCS})