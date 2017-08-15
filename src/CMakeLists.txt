#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目

if(MSVC)
	ADD_LIBRARY(parser SHARED jsonexport.h jsondef.h jsonparser.hpp jsonsrc.cpp jsonvalue.hpp export.def )	
else()
	ADD_LIBRARY(parser SHARED jsonexport.h jsondef.h jsonparser.hpp jsonsrc.cpp jsonvalue.hpp )
endif()

install(TARGETS parser
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
)
install(DIRECTORY ./ DESTINATION include/parser
         FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY ./ DESTINATION include/parser
         FILES_MATCHING PATTERN "*.h")