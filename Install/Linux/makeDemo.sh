#!/bin/sh
# Exit script on error
set -e

# Set the working directory correctly
cd "$(dirname "$0")"

# **************************************************************************
# CMAKE GENERATION
# **************************************************************************
GV_ROOT=`(cd ../.. && pwd)`

# Create directory
mkdir -p $GV_ROOT/Generated_Linux/Tutorials/Demos

# Generates standard UNIX makefiles
cd $GV_ROOT/Generated_Linux/Tutorials/Demos
cmake -G "Unix Makefiles" $GV_ROOT/Development/Tutorials/Demos
