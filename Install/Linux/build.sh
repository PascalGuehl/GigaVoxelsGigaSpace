#!/bin/sh
# Exit script on error
set -e

# Set the working directory correctly
cd "$(dirname "$0")"
GV_ROOT=`(cd ../.. && pwd)`

usage() {
	echo Usage :
	echo "$0 [N_PROCS]"
}

# Check arguments
if [ $# -eq 1 ]; then
	# Check $1 is a number
	case "$1" in
		''|*[!0-9]*) 
			echo "Error $1 is not a number."
			usage $0
			exit 1
		;;
		*);;
	esac
	echo "$1" threads will be used to compile.
	N_PROCS=$1
elif [ $# -ge 2 ]; then
	echo "Error, too many arguments."
	usage $0
	exit 1
else 
	echo "No number provided, default number of processors (4) will be used"
	N_PROCS=4
fi;

# **************************************************************************
# BUILD EVERYTHING
# **************************************************************************

# Build externals libraries - cudpp
rm -rf $GV_ROOT/External/CommonLibraries/cudpp-2.1/build
mkdir $GV_ROOT/External/CommonLibraries/cudpp-2.1/build
cd $GV_ROOT/External/CommonLibraries/cudpp-2.1/build
cmake ..
make -j "$N_PROCS"

mkdir -p $GV_ROOT/External/Linux/x64/cudpp/lib
mkdir -p $GV_ROOT/External/Linux/x64/cudpp/include
cp $GV_ROOT/External/CommonLibraries/cudpp-2.1/build/lib/libcudpp.so $GV_ROOT/External/Linux/x64/cudpp/lib
cp $GV_ROOT/External/CommonLibraries/cudpp-2.1/include/* $GV_ROOT/External/Linux/x64/cudpp/include

# Build externals libraries - glew
cd $GV_ROOT/External/CommonLibraries/glew-1.12.0
chmod +x config/config.guess
make -j "$N_PROCS"
mkdir -p $GV_ROOT/External/Linux/x64/glew/lib
mkdir -p $GV_ROOT/External/Linux/x64/glew/include/GL
cp $GV_ROOT/External/CommonLibraries/glew-1.12.0/lib/libGLEW.a $GV_ROOT/External/Linux/x64/glew/lib
cp $GV_ROOT/External/CommonLibraries/glew-1.12.0/include/GL/* $GV_ROOT/External/Linux/x64/glew/include/GL

# Build GigaSpace
make -i -C $GV_ROOT/Generated_Linux/Library -j "$N_PROCS"
make -i -C $GV_ROOT/Generated_Linux/Tools -j "$N_PROCS"
make -i -C $GV_ROOT/Generated_Linux/Tutorials/Demos -j "$N_PROCS"
make -i -C $GV_ROOT/Generated_Linux/Tutorials/ViewerPlugins -j "$N_PROCS"
