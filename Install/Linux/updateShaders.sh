#!/bin/sh
# Exit script on error
set -e

# Set the working directory
cd "$(dirname "$0")"

# **************************************************************************
# Setups environment variables
# **************************************************************************
GV_ROOT=`(cd ../.. && pwd)`

# PATH : GigaVoxels RELEASE directory
GV_RELEASE=$GV_ROOT/Release

# PATH : GigaVoxels Development directory
GV_DATA=$GV_ROOT/Development

# **************************************************************************
# GLSL shaders
# **************************************************************************

# Demos
mkdir -p $GV_RELEASE/Bin/Data/Shaders/SimpleSphere
cp -L -u -R $GV_DATA/Tutorials/Demos/ProceduralTechnics/SimpleSphere/Res/*.* $GV_RELEASE/Bin/Data/Shaders/SimpleSphere/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/ProxyGeometry
cp -L -u -R $GV_DATA/Tutorials/Demos/GraphicsInteroperability/ProxyGeometry/Res/*.* $GV_RELEASE/Bin/Data/Shaders/ProxyGeometry/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/RendererGLSL
cp -L -u -R $GV_DATA/Tutorials/Demos/GraphicsInteroperability/RendererGLSL/Res/*.* $GV_RELEASE/Bin/Data/Shaders/RendererGLSL/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/RendererGLSLSphere
cp -L -u -R $GV_DATA/Tutorials/Demos/GraphicsInteroperability/RendererGLSLSphere/Res/*.* $GV_RELEASE/Bin/Data/Shaders/RendererGLSLSphere/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/Voxelization
cp -L -u -R $GV_DATA/Tutorials/Demos/Voxelization/Voxelization/Res/*.* $GV_RELEASE/Bin/Data/Shaders/Voxelization/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/VoxelizationSignedDistanceField
cp -L -u -R $GV_DATA/Tutorials/Demos/Voxelization/VoxelizationSignedDistanceField/Res/*.* $GV_RELEASE/Bin/Data/Shaders/VoxelizationSignedDistanceField/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/CastShadows
cp -L -u -R $GV_DATA/Tutorials/Demos/GraphicsInteroperability/CastShadows/Res/*.* $GV_RELEASE/Bin/Data/Shaders/CastShadows/.

# Viewer plugins
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvAmplifiedSurface
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvAmplifiedSurface/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvAmplifiedSurface/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvAmplifiedVolume
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvAmplifiedVolume/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvAmplifiedVolume/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvDynamicLoad
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvDynamicLoad/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvDynamicLoad/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvDepthPeeling
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvDepthPeeling/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvDepthPeeling/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvProxyGeometryManager
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvProxyGeometryManager/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvProxyGeometryManager/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvRayMapGenerator
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvRayMapGenerator/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvRayMapGenerator/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvRendererGLSL
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvRendererGLSL/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvRendererGLSL/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvSimpleShapeGLSL
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvSimpleShapeGLSL/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvSimpleShapeGLSL/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvSignedDistanceFieldVoxelization
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvSignedDistanceFieldVoxelization/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvSignedDistanceFieldVoxelization/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvSimpleSphere
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvSimpleSphere/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvSimpleSphere/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvVBOGenerator
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvVBOGenerator/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvVBOGenerator/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvAnimatedLUT
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvAnimatedLUT/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvAnimatedLUT/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvInstancing
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvInstancing/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvInstancing/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvSlisesix
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvInstancing/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvSlisesix/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvEnvironmentMapping
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvEnvironmentMapping/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvEnvironmentMapping/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvAnimatedSnake
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvAnimatedSnake/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvAnimatedSnake/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvCastShadows
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvCastShadows/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvCastShadows/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvLazyHypertexture
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvLazyHypertexture/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvLazyHypertexture/.
mkdir -p $GV_RELEASE/Bin/Data/Shaders/GvProductionPolicies
cp -L -u -R $GV_DATA/Tutorials/ViewerPlugins/GvProductionPolicies/Res/*.* $GV_RELEASE/Bin/Data/Shaders/GvProductionPolicies/.
