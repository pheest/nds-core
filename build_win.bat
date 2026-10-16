@echo off
REM The fully-qualified path to VS-Studio installed CMake is used because (an ealier version) is also likely to be installed with Strawberry Perl.
REM The latter is likely to be in PATH and does not accept use of the x64 architecture parameter.
set CMAKE="C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

%CMAKE% -B build -S . -DBUILD_TESTING=ON -DBUILD_EXAMPLES=ON -A x64

REM Build for both Release and Debug configurations.
%CMAKE% --build build --config Release
%CMAKE% --build build --config Debug

REM Install both Release and Debug builds.
%CMAKE% --install build --config Release --prefix C:/nds3/
%CMAKE% --install build --config Debug --prefix C:/nds3/

REM Windows-specific RELEASE files in NDS3 dependant projects will then specify e.g. NDS3=C:/nds3
