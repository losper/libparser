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
ADD_EXECUTABLE(libparsertest ../src/xmlparser.hpp captest.cpp ../../Aeolus/src/aeolus.hpp)
#ADD_EXECUTABLE(test_mcuparser mcuparser_test.cpp logic.cpp logic.h)
#target_link_libraries(test_simple capture)
#target_link_libraries(meta avcodec avformat avutil avdevice avfilter swscale)
#target_link_libraries(d2e avcodec avformat avutil avdevice avfilter swscale)
#target_link_libraries(d2d avcodec avformat avutil avdevice avfilter swscale)
