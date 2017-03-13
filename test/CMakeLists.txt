#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(../src ../../Aeolus/src)

## 添加项目
#ADD_EXECUTABLE(test_simple simple.cpp)
#ADD_EXECUTABLE(meta metadata.c)
#ADD_EXECUTABLE(d2e decoding_encoding.c)
#ADD_EXECUTABLE(d2d demuxing_decoding.c)
ADD_EXECUTABLE(libparsertest simple.cpp)
