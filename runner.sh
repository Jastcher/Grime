#! /bin/bash

mkdir -p Build >/dev/null 2>&1
dir=$(dirname $(which $0 2>/dev/null || realpath ./$0))

cd $dir/Build
cmake ../
mv compile_commands.json ../

cmake --build .

cd ../bin/
if [ $1 ]; then
	./Application
fi

cd ..
