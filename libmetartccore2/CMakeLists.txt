cmake_minimum_required(VERSION 2.8)
project(libmetartccore2)

add_definitions(-D__STDC_FORMAT_MACROS)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -ffunction-sections -fdata-sections")
set(HOME_BASE "../")

# 头文件目录
include_directories(${HOME_BASE}/include)
include_directories(${HOME_BASE}/thirdparty/include)
include_directories(${HOME_BASE}/thirdparty/user_include)
include_directories(${HOME_BASE}/thirdparty/user_include/ffmpeg)
include_directories(${HOME_BASE}/libmetartccore2/src)


# 发现目录下的源文件

aux_source_directory(./src/yangutil/sys DIR_SRCS)
aux_source_directory(./src/yangavutil DIR_SRCS)
aux_source_directory(./src/yangcsrs DIR_SRCS)
aux_source_directory(./src/yangsdp DIR_SRCS)
aux_source_directory(./src/yangrtp DIR_SRCS)
aux_source_directory(./src/yangwebrtc DIR_SRCS)
include(utils.cmake)
# 排除不参与编译的文件


add_library(metartccore2 ${DIR_SRCS})
