@echo off

rem **************************************************************************
rem Setups environment variables
rem **************************************************************************

rem PATH : GigaVoxels RELEASE directory
SET GV_RELEASE=%CD%\..\..\Release

rem PATH : GigaVoxels Development directory
SET GV_DATA=%CD%\..\..\Development

rem **************************************************************************
rem GLSL shaders
rem **************************************************************************

rem Demos
xcopy /y /c /d "%GV_DATA%\Tutorials\Demos\ProceduralTechnics\SimpleSphere\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\SimpleSphere\"
xcopy /y /c /d "%GV_DATA%\Tutorials\Demos\GraphicsInteroperability\ProxyGeometry\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\ProxyGeometry\"
xcopy /y /c /d "%GV_DATA%\Tutorials\Demos\GraphicsInteroperability\RendererGLSL\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\RendererGLSL\"
xcopy /y /c /d "%GV_DATA%\Tutorials\Demos\GraphicsInteroperability\RendererGLSLSphere\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\RendererGLSLSphere\"
xcopy /y /c /d "%GV_DATA%\Tutorials\Demos\Voxelization\Voxelization\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\Voxelization\"
xcopy /y /c /d "%GV_DATA%\Tutorials\Demos\Voxelization\VoxelizationSignedDistanceField\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\VoxelizationSignedDistanceField\"
xcopy /y /c /d "%GV_DATA%\Tutorials\Demos\GraphicsInteroperability\CastShadows\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\CastShadows\"

rem Viewer plugins
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvAmplifiedSurface\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvAmplifiedSurface\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvAmplifiedVolume\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvAmplifiedVolume\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvDynamicLoad\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvDynamicLoad\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvDepthPeeling\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvDepthPeeling\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvProxyGeometryManager\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvProxyGeometryManager\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvRayMapGenerator\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvRayMapGenerator\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvRendererGLSL\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvRendererGLSL\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvSimpleShapeGLSL\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvSimpleShapeGLSL\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvShadowMap\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvShadowMap\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvSignedDistanceFieldVoxelization\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvSignedDistanceFieldVoxelization\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvSimpleSphere\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvSimpleSphere\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvVBOGenerator\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvVBOGenerator\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvAnimatedLUT\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvAnimatedLUT\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvAnimatedSnake\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvAnimatedSnake\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvInstancing\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvInstancing\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvSlisesix\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvSlisesix\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvEnvironmentMapping\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvEnvironmentMapping\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvAnimatedSnake\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvAnimatedSnake\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvShadowCasting\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvShadowCasting\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvCastShadows\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvCastShadows\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvNoiseInAShellGLSL\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvNoiseInAShellGLSL\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvLazyHypertexture\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvLazyHypertexture\"
xcopy /y /c /d "%GV_DATA%\Tutorials\ViewerPlugins\GvProductionPolicies\Res\*.*" "%GV_RELEASE%\Bin\Data\Shaders\GvProductionPolicies\"

pause
