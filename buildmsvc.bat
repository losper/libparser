echo "generate this project"

rd msvcbuild /S/Q
mkdir msvcbuild
cd msvcbuild

set GENERATOR=Visual Studio 14 2015 Win64
set CONFIG=Release
cmake ../ -G "%GENERATOR%"
pause