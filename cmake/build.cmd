set PATH=D:\Qt\Qt5.7.0\Tools\mingw530_32\bin;%PATH%

cmake^
-G "CodeBlocks - MinGW Makefiles"^
-D CMAKE_PREFIX_PATH="D:\Qt\Qt5.7.0\5.7\mingw53_32"^
..\GraphViewer

mingw32-make all
