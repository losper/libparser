
rd /S /Q imx6qwinbuild
mkdir imx6qwinbuild
cd imx6qwinbuild
cmake ../ -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../cmake/imx6qwin.cmake -DCMAKE_INSTALL_PREFIX=e:/usr/linaro/imx6q
cmake --build ./ --target install

pause