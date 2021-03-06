#!/usr/bin/env bash
# using the above shebang to be most-portable
# https://stackoverflow.com/questions/10376206/what-is-the-preferred-bash-shebang

# notes:
# mkdir -p creates a nested directory structure
# https://linux.die.net/man/1/mkdir
# && is used to string together commands such that cmd1 && cmd2 will only run cmd2 if cmd1 succeeds
# https://stackoverflow.com/questions/5130847/running-multiple-commands-in-one-line-in-shell
# cmake -G sets the generator to use
# cmake -D sets a variable in the cmake cache (here I set the build config for the specific makefile that gets created)
# https://cmake.org/cmake/help/v3.2/manual/cmake.1.html

# Get dependency if they are not in place
if [ ! "$(ls -A deps/cotire)" ] 
then
	echo "Installing cotire"
	git clone https://github.com/sakra/cotire deps/cotire
fi

if [ ! "$(ls -A deps/googletest)" ] 
then
	echo "Installing googletest"
	git clone https://github.com/google/googletest deps/googletest
fi

echo "BUILDING DEBUG CONFIG..."
mkdir -p build/Unix-x64/Debug && cd build/Unix-x64/Debug && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../../.. && make && cd ../../..

#echo "BUILDING MINSIZEREL CONFIG..."
#mkdir -p build/Unix-x64/MinSizeRel && cd build/Unix-x64/MinSizeRel && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel ../../.. && make && cd ../../..

#echo "BUILDING RELEASE CONFIG..."
#mkdir -p build/Unix-x64/Release && cd build/Unix-x64/Release && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../../.. && make && cd ../../..

#echo "BUILDING RELWITHDEBINFO CONFIG..."
#mkdir -p build/Unix-x64/RelWithDebInfo && cd build/Unix-x64/RelWithDebInfo && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo ../../.. && make && cd ../../..

# pause the script at the end, until user wants to close it (analog to DOS-pause)
# https://stackoverflow.com/questions/92802/what-is-the-linux-equivalent-to-dos-pause
read -n1 -r -p "Press any key to continue . . . " key
