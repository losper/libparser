if(CMAKE_INCLUDE_PREFIX)
else()
set(CMAKE_INCLUDE_PREFIX e:/usr/local)
endif()
include_directories(${CMAKE_INCLUDE_PREFIX}/include)
LINK_DIRECTORIES (${CMAKE_INCLUDE_PREFIX}/lib)