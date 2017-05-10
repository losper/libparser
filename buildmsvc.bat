echo "generate this project"
rd msvcbuild /S/Q
mkdir msvcbuild
cd msvcbuild
rem cmake ../ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug

set GENERATOR=Visual Studio 14 2015 Win64
set CONFIG=Release
if '%1'=='' goto :yes
goto :no
:yes
cmake ../ -G "%GENERATOR%" -DCMAKE_TOOLCHAIN_FILE=../cmake/msvc.cmake
cmake --build ./ --target INSTALL
goto :eof

:no
cmake ../ -G %1 %3 %4 %5
if %2==1 cmake --build ./ --target INSTALL
if %errorlevel% equ 0 goto :eof
