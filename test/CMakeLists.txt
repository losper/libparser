#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)


## 添加项目

ADD_EXECUTABLE(libparsertest simple.cpp)
ADD_EXECUTABLE(libparserhpptest hpptest.cpp)
target_link_libraries(libparsertest parser)

if(MSVC)
set(WINTEST libparsertest libparserhpptest)
set_target_properties(${WINTEST} PROPERTIES FOLDER "tests/parser")
endif()