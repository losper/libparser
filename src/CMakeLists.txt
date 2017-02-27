#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(${CMAKE_LOCAL_DIRECTORY}/include)
LINK_DIRECTORIES (${CMAKE_LOCAL_DIRECTORY}/lib)

## 添加项目

if(MSVC)
	ADD_LIBRARY(capture SHARED capture.h capture.cpp test.cpp export.def)	
else()
	ADD_LIBRARY(capture SHARED capture.h capture.cpp)	
endif()
target_link_libraries(capture avcodec avformat avutil avdevice avfilter swscale)