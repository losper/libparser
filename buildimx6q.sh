rm -rf imx6qbuild
mkdir imx6qbuild
cd imx6qbuild
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../cmake/imx6q.cmake -DCMAKE_INSTALL_PREFIX=~/dev/arm-linux-gnueabihf
#cmake ../ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build ./ --target install
