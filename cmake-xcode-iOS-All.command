#! /bin/bash
cd `dirname $0`

if [ ! -d "Build" ]; then
	mkdir Build
fi
cd Build
if [ -d "xcode-iOS" ]; then
	rm -Rf xcode-iOS
fi
mkdir xcode-iOS
cd xcode-iOS
cmake -DCMAKE_TOOLCHAIN_FILE="CMake/iOS.toolchain.cmake" -DUSE_LUAJIT=0 -DBUILD_RUNTIME=1 -DBUILD_TEST=1 -DBUILD_PLAYER=1 -DBS_USE_ADMOB=FALSE -G "Xcode" ../..
