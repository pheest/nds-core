# Building NDS on Windows

## Use of vcpkg

vcpkg is installed (by default) with the Visual Studio C++ workflow.
However, the version supplied with VS prohibits use of 'Classic Mode' and only works with 'Manifest Mode'.

Commands like vcpkg install dlfcn-win32:x64-windows pthreads:x64-windows are using 'Classic Mode'.

To enable use of 'Classic Mode', clone the https://github.com/microsoft/vcpkg.git repository to (e.g. C:\vcpkg) and run bootstrap-vcpkg.bat.
This is followed by 'vcpkg integrate install'. This command needs to be invoked only once.

To install the required libraries from the cmd shell:
vcpkg install dlfcn-win32:x64-windows pthreads:x64-windows for the shared library installs and
vcpkg install dlfcn-win32:x64-windows-static pthreads:x64-windows-static for the static library installs.

## Use of CMake

CMake is installed (by default) with the Visual Studio C++ workflow.
It is also bundled with Stawberry Perl.

The vcpkg installer will likely also have installed it's own (and latest) version.
This version (and not either of the other two above) should be used for the NDS build.
