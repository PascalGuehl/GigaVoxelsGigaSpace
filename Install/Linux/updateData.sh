#!/bin/sh
# Exit script on error
set -e

# Set the working directory correctly
cd "$(dirname "$0")"

# **************************************************************************
# Setups environment variables
# **************************************************************************
GV_ROOT=`(cd ../.. && pwd)`

# PATH : GigaVoxels RELEASE directory
GV_RELEASE=$GV_ROOT/Release

# PATH : GigaVoxels DATA directory
GV_DATA=$GV_ROOT/Data

# **************************************************************************
# Icons
# **************************************************************************
mkdir -p $GV_RELEASE/Bin/Resources/Icons
cp -L -u -R $GV_DATA/Icons/*.* $GV_RELEASE/Bin/Resources/Icons/.

# **************************************************************************
# Settings
# **************************************************************************
mkdir -p $GV_RELEASE/Bin/Settings
cp -L -u -R $GV_DATA/Settings/* $GV_RELEASE/Bin/Settings/

# **************************************************************************
# Shaders
# **************************************************************************
mkdir -p $GV_RELEASE/Bin/Data/Shaders
cp -L -u -R $GV_DATA/Shaders/*.* $GV_RELEASE/Bin/Data/Shaders/.

# **************************************************************************
# TransferFunctions
# **************************************************************************
mkdir -p $GV_RELEASE/Bin/Data/TransferFunctions
cp -L -u -R $GV_DATA/TransferFunctions/*.* $GV_RELEASE/Bin/Data/TransferFunctions/.

# **************************************************************************
# Voxels
# **************************************************************************
mkdir -p $GV_RELEASE/Bin/Data/Voxels/xyzrgb_dragon512_BR8_B1
cp -L -u -R $GV_DATA/Voxels/xyzrgb_dragon512_BR8_B1/*.* $GV_RELEASE/Bin/Data/Voxels/xyzrgb_dragon512_BR8_B1/.

mkdir -p $GV_RELEASE/Bin/Data/Voxels/Dino
cp -L -u -R $GV_DATA/Voxels/Dino/*.* $GV_RELEASE/Bin/Data/Voxels/Dino/.

mkdir -p $GV_RELEASE/Bin/Data/Voxels/Fan
cp -L -u -R $GV_DATA/Voxels/Fan/*.* $GV_RELEASE/Bin/Data/Voxels/Fan/.

mkdir -p $GV_RELEASE/Bin/Data/Voxels/vd4
cp -L -u -R $GV_DATA/Voxels/vd4/*.* $GV_RELEASE/Bin/Data/Voxels/vd4/.

mkdir -p $GV_RELEASE/Bin/Data/Voxels/Raw/aneurism
cp -L -u -R $GV_DATA/Voxels/Raw/aneurism/*.* $GV_RELEASE/Bin/Data/Voxels/Raw/aneurism/.
mkdir -p $GV_RELEASE/Bin/Data/Voxels/Raw/bonsai
cp -L -u -R $GV_DATA/Voxels/Raw/bonsai/*.* $GV_RELEASE/Bin/Data/Voxels/Raw/bonsai/.
mkdir -p $GV_RELEASE/Bin/Data/Voxels/Raw/foot
cp -L -u -R $GV_DATA/Voxels/Raw/foot/*.* $GV_RELEASE/Bin/Data/Voxels/Raw/foot/.
mkdir -p $GV_RELEASE/Bin/Data/Voxels/Raw/hydrogenAtom
cp -L -u -R $GV_DATA/Voxels/Raw/hydrogenAtom/*.* $GV_RELEASE/Bin/Data/Voxels/Raw/hydrogenAtom/.
mkdir -p $GV_RELEASE/Bin/Data/Voxels/Raw/neghip
cp -L -u -R $GV_DATA/Voxels/Raw/neghip/*.* $GV_RELEASE/Bin/Data/Voxels/Raw/neghip/.
mkdir -p $GV_RELEASE/Bin/Data/Voxels/Raw/skull
cp -L -u -R $GV_DATA/Voxels/Raw/skull/*.* $GV_RELEASE/Bin/Data/Voxels/Raw/skull/.

# **************************************************************************
# 3D Models
# **************************************************************************
# Unzip models
#(cd $GV_DATA/3DModels/ && unzip -f stanford_dragon.zip > /dev/null)
#(cd $GV_DATA/3DModels/ && unzip -f stanford_buddha.zip > /dev/null)
#(cd $GV_DATA/3DModels/ && unzip -f stanford_bunny.zip > /dev/null)

mkdir -p $GV_RELEASE/Bin/Data/3DModels
cp -L -u -R $GV_DATA/3DModels/*.* $GV_RELEASE/Bin/Data/3DModels/.

mkdir -p $GV_RELEASE/Bin/Data/3DModels/stanford_buddha
cp -L -u -R $GV_DATA/3DModels/buddha.obj $GV_RELEASE/Bin/Data/3DModels/stanford_buddha/.
mkdir -p $GV_RELEASE/Bin/Data/3DModels/stanford_bunny
cp -L -u -R $GV_DATA/3DModels/bunny.obj $GV_RELEASE/Bin/Data/3DModels/stanford_bunny/.
cp -L -u -R $GV_DATA/3DModels/bunny.obj $GV_RELEASE/Bin/Data/3DModels/.
mkdir -p $GV_RELEASE/Bin/Data/3DModels/stanford_dragon
cp -L -u -R $GV_DATA/3DModels/dragon.obj $GV_RELEASE/Bin/Data/3DModels/stanford_dragon/.

# **************************************************************************
# Videos
# **************************************************************************
mkdir -p $GV_RELEASE/Bin/Data/Videos
