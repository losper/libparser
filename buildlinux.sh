rm -rf linuxbuild
mkdir linuxbuild
cd linuxbuild
cmake ../ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#cmake --build ./ --target install
