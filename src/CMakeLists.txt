#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目

if(MSVC)
	ADD_LIBRARY(parser SHARED jsonexport.h jsonparser.h jsonparser.cpp jsonvalue.h jsonvalue.cpp export.def)	
else()
	ADD_LIBRARY(parser SHARED jsonexport.h jsonparser.h jsonparser.cpp jsonvalue.h jsonvalue.cpp)
endif()

install(TARGETS parser
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
)
install(DIRECTORY ./ DESTINATION include/parser
         FILES_MATCHING PATTERN "*.h")