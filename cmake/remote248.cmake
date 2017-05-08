# this is required
SET(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# specify the cross compiler
SET(CMAKE_C_COMPILER /opt/Sourcery_CodeBench_for_ARM_Embedded/bin/arm-none-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER /opt/Sourcery_CodeBench_for_ARM_Embedded/bin/arm-none-linux-gnueabi-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /opt/Sourcery_CodeBench_for_ARM_Embedded/)
SET(CMAKE_EXT_LIBRARAY_PATH  /home/hs/workspace/build/tmp/sysroots/mx6q/usr)
# search for programs in the build host directories (not necessary)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# configure Boost and Qt
#SET(QT_QMAKE_EXECUTABLE /opt/qt-embedded/qmake)
#SET(BOOST_ROOT /opt/boost_arm)

if(CMAKE_INCLUDE_PREFIX)
include_directories(${CMAKE_INCLUDE_PREFIX}/include)
LINK_DIRECTORIES (${CMAKE_INCLUDE_PREFIX}/lib)
endif()