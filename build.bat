echo "generate this project"

rd build /S/Q
mkdir build
cd build
rem cmake ../ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug

set GENERATOR=Visual Studio 14 2015 Win64
set CONFIG=Release
cmake ../ -G "%GENERATOR%"
pause