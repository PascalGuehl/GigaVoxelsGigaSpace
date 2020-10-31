;--------------------------------
; GIGAVOXELS INSTALL SCRIPT
;--------------------------------

!include "FileFunc.nsh"
!include "MUI.nsh"

; The product information
Name "GigaVoxels SDK"

; The output file to write
OutFile "GigaVoxels_v100.exe"

; The default installation directory
InstallDir "$PROGRAMFILES\GigaVoxels_v100"

;--------------------------------
; PAGES
;--------------------------------

; Settings  
!define MUI_LICENSEPAGE_RADIOBUTTONS

; Installer pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "License.rtf"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Language
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"

; Language selection dialog
Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function un.onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

;--------------------------------
; SECTION
;--------------------------------

Section "GigaVoxels Library" GsLibrary

  SetOverwrite ifnewer
  
  ; Add system files
  SetOutPath $INSTDIR
  File /x *.d.dll /x *d4.dll /x *d2.dll /x *32d.dll /x 64d.dll ..\Release\Bin\*.dll
  File /x *.d.exe ..\Release\Bin\*.exe
  File /x *.d.gvp ..\Release\Bin\*.gvp

  ; Add CUDA shared library
  ; - file should be located here : C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.0\bin
  ; File $CUDA_PATH\bin\cudart64_70.dll"
  File "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.0\bin\cudart64_70.dll"
    
  ; Add Settings dir
  SetOutPath $INSTDIR\Settings
  File ..\Release\Bin\Settings\*.*
  
  ; Add manuals dir
  ;SetOutPath $INSTDIR\Manuals
  ;File /r /x .svn ..\Release\Bin\Manuals\*.*
    
  ; Add Tools dir
  ; --- viewer
  ;File /x *.d.dll ..\Release\Tools\GigaVoxelsViewer\Bin\*.dll
  ;File /x *.d.exe ..\Release\Tools\GigaVoxelsViewer\Bin\*.exe
  ; --- voxelizer
  ;File /x *.d.exe ..\Release\Tools\GigaVoxelsVoxelizer\Bin\*.exe
  
  ; Add icons directory
  SetOutPath $INSTDIR\Resources\Icons
  File ..\Release\Bin\Resources\Icons\*.*
  
  ; Add voxelized models directory
  ;SetOutPath $INSTDIR\Data\Voxels\vd4
  ;File ..\Release\Bin\Data\Voxels\vd4\*.*
  ;SetOutPath $INSTDIR\Data\Voxels\xyzrgb_dragon512_BR8_B1
  ;File ..\Release\Bin\Data\Voxels\xyzrgb_dragon512_BR8_B1\*.*
  ;SetOutPath $INSTDIR\Data\Voxels\Dino
  ;File ..\Release\Bin\Data\Voxels\Dino\*.*
  ;SetOutPath $INSTDIR\Data\Voxels\Fan
  ;File ..\Release\Bin\Data\Voxels\Fan\*.*
  
  ; Add 3D models directory
  ;SetOutPath $INSTDIR\Data
  ;File ..\Data\ReadMe.txt
  ;SetOutPath $INSTDIR\Data\3DModels
  ;File ..\Release\Bin\Data\3DModels\*.*
  
  ; Add shaders directory
  ; TODO : check each demo ?
  SetOutPath $INSTDIR\Data\Shaders
  File /r ..\Release\Bin\Data\Shaders\*.*
     
  ; Add transfer function directory
  SetOutPath $INSTDIR\Data\TransferFunctions
  File ..\Release\Bin\Data\TransferFunctions\*.*
  
  ; Add SkyBox directory
  ;SetOutPath $INSTDIR\Data\SkyBox
  ;File ..\Release\Bin\Data\SkyBox\*.*
  
  ; Add Terrain directory
  ;SetOutPath $INSTDIR\Data\Terrain
  ;File ..\Release\Bin\Data\Terrain\*.*
  
  ; Write the uninstaller
  WriteUninstaller "uninstall.exe"
  
SectionEnd

;--------------------------------
; SECTION
;--------------------------------

!define TOOL_GvViewer
!define TOOL_GvVoxelizer
!define TOOL_GvDicomVoxelizer
!define SECTION_TOOLS

!ifdef SECTION_TOOLS
  SectionGroup "Tools" GvTools
!endif
  !ifdef TOOL_GvViewer
    Section "GvViewer" GvViewer
      SetOutPath $INSTDIR
      File ..\Release\Tools\GigaVoxelsViewer\Bin\GvViewer.exe
	  File ..\Release\Tools\GigaVoxelsViewer\Bin\GvViewerCore.dll
	  File ..\Release\Tools\GigaVoxelsViewer\Bin\GvViewerGui.dll
    SectionEnd
  !endif
  !ifdef TOOL_GvVoxelizer
    Section "GvVoxelizer" GvVoxelizer
      SetOutPath $INSTDIR
      File ..\Release\Tools\GigaVoxelsVoxelizer\Bin\GvVoxelizer.exe
	SectionEnd
  !endif
  !ifdef TOOL_GvDicomVoxelizer
    Section "GvDicomVoxelizer" GvDicomVoxelizer
      SetOutPath $INSTDIR
      File ..\Release\Tools\GigaVoxelsVoxelizer\Bin\GvDicomVoxelizer.exe
	SectionEnd
  !endif
!ifdef SECTION_TOOLS
  SectionGroupEnd
!endif

;--------------------------------
; SECTION
;--------------------------------

!define PRODUCER_GvAmplifiedSurface
!define PRODUCER_GvAmplifiedVolume
!define PRODUCER_GvAnimatedFan
!define PRODUCER_GvAnimatedSnake
!define PRODUCER_GvCastShadows
!define PRODUCER_GvCollision
!define PRODUCER_GvCollisionBBox
!define PRODUCER_GvDicomViewer
!define PRODUCER_GvDynamicHypertexture
!define PRODUCER_GvDynamicLoad
!define PRODUCER_GvEnvironmentMapping
!define PRODUCER_GvHypertexture
!define PRODUCER_GvInfiniteSky
!define PRODUCER_GvInstancing
!define PRODUCER_GvLazyHypertexture
!define PRODUCER_GvMandelbrotSet
!define PRODUCER_GvMengerSponge
!define PRODUCER_GvNoiseInAShellGLSL
!define PRODUCER_GvNoiseInheritance
!define PRODUCER_GvProductionPolicies
!define PRODUCER_GvProxyGeometryManager
!define PRODUCER_GvRAWDataLoader
!define PRODUCER_GvRendererGLSL
!define PRODUCER_GvShellNoise
!define PRODUCER_GvSignedDistanceFieldVoxelization
!define PRODUCER_GvSimpleShapeGLSL
!define PRODUCER_GvSimpleSphere
!define PRODUCER_GvSimpleSphereCPU
!define PRODUCER_GvSlisesix
!define PRODUCER_GvSphereRayTracing
!define PRODUCER_GvVBOGenerator

!define SECTION_PRODUCERS

!ifdef SECTION_PRODUCERS
  SectionGroup "Tutorials" GvProducers
!endif
	!ifdef PRODUCER_GvAmplifiedSurface
    Section "Amplified Surface" GvAmplifiedSurface
      SetOutPath $INSTDIR
      File ..\Release\Bin\GvAmplifiedSurface.gvp
    SectionEnd
  !endif
  !ifdef PRODUCER_GvAmplifiedVolume
    Section "Amplified Volume" GvAmplifiedVolume
      SetOutPath $INSTDIR
      File ..\Release\Bin\GvAmplifiedVolume.gvp
    SectionEnd
  !endif
  !ifdef PRODUCER_GvDynamicLoad
    Section "Dynamic Load" GvDynamicLoad
      SetOutPath $INSTDIR
      File ..\Release\Bin\GvDynamicLoad.gvp
	SectionEnd
  !endif
  !ifdef PRODUCER_GvHypertexture
    Section "Hypertexture" GvHypertexture
      SetOutPath $INSTDIR
      File ..\Release\Bin\GvHypertexture.gvp
	SectionEnd
  !endif
  !ifdef PRODUCER_GvMandelbrotSet
    Section "Mandelbulb" GvMandelbrotSet
      SetOutPath $INSTDIR
      File ..\Release\Bin\GvMandelbrotSet.gvp
	SectionEnd
  !endif
  !ifdef PRODUCER_GvMengerSponge
    Section "Menger Sponge" GvMengerSponge
      SetOutPath $INSTDIR
      File ..\Release\Bin\GvMengerSponge.gvp
	SectionEnd
  !endif
  !ifdef PRODUCER_GvRAWDataLoader
    Section "RAW Data Loader" GvRAWDataLoader
      SetOutPath $INSTDIR
      File ..\Release\Bin\GvRAWDataLoader.gvp
	SectionEnd
  !endif
  !ifdef PRODUCER_GvRendererGLSL
    Section "Renderer GLSL" GvRendererGLSL
      SetOutPath $INSTDIR
      File ..\Release\Bin\GvRendererGLSL.gvp
	SectionEnd
  !endif
  !ifdef PRODUCER_GvSimpleSphere
    Section "Simple Sphere" GvSimpleSphere
      SetOutPath $INSTDIR
      File ..\Release\Bin\GvSimpleSphere.gvp
	SectionEnd
  !endif
!ifdef SECTION_PRODUCERS
  SectionGroupEnd
!endif

;--------------------------------
; SECTION
;--------------------------------

Section "Raccourcis Menu Démarrer" Section_Start_Menu

;-- Writes the shortcuts
  SetOutPath $INSTDIR
  CreateDirectory "$SMPROGRAMS\GigaVoxels"
  CreateShortCut "$SMPROGRAMS\GigaVoxels\GigaVoxels viewer.lnk" "$INSTDIR\GvViewer.exe"
  CreateShortCut "$SMPROGRAMS\GigaVoxels\GigaVoxels voxelizer.lnk" "$INSTDIR\GvVoxelizer.exe"
  CreateShortCut "$SMPROGRAMS\GigaVoxels\GigaVoxels DICOM voxelizer.lnk" "$INSTDIR\GvDicomVoxelizer.exe"
  CreateShortCut "$SMPROGRAMS\GigaVoxels\Manuel utilisateur.lnk" "$INSTDIR\Manuals\fr\GigaVoxels.htm"
  CreateShortCut "$SMPROGRAMS\GigaVoxels\Désinstaller GigaVoxels.lnk" "$INSTDIR\uninstall.exe"
  
SectionEnd

;--------------------------------
; SECTION
;--------------------------------

Section "Raccourci Bureau" Section_Desktop_Shortcut

  CreateShortCut "$DESKTOP\GigaVoxels.lnk" "$INSTDIR\GvViewer.exe"
  
SectionEnd

;--------------------------------
; DESCRIPTION SECTIONS
;--------------------------------

; English
; --- library
LangString DESC_SECTION_LIBRARY ${LANG_ENGLISH} "The GigaVoxels library. DLL and headers files."
; --- tools
LangString DESC_SECTION_TOOLS ${LANG_ENGLISH} "The GigaVoxels tools."
LangString DESC_TOOL_GvViewer ${LANG_ENGLISH} "The GigaVoxels viewer is used to launch all producers and visualize data interactively."
LangString DESC_TOOL_GvVoxelizer ${LANG_ENGLISH} "The GigaVoxels voxelizer is used to pre-process data in order to visualize them with GigaVoxels."
LangString DESC_TOOL_GvDicomVoxelizer ${LANG_ENGLISH} "The GigaVoxels Dicom voxelizer is used to pre-process Dicom data in order to visualize them with GigaVoxels."
; --- producers
LangString DESC_SECTION_PRODUCERS ${LANG_ENGLISH} "The GigaVoxels producers."
LangString DESC_PRODUCER_GvAmplifiedVolume ${LANG_ENGLISH} "Signed distance field Bunny with a turbulence Perlin noise."
LangString DESC_PRODUCER_GvDynamicLoad ${LANG_ENGLISH} "I/O streaming data on disk to load a 3D model."

; French
; --- library
LangString DESC_SECTION_LIBRARY ${LANG_FRENCH} "La librairie GigaVoxels. DLL et fichiers header."
; --- tools
LangString DESC_SECTION_TOOLS ${LANG_FRENCH} "Les outils GigaVoxels."
LangString DESC_TOOL_GvViewer ${LANG_FRENCH} "Le visualiseur GigaVoxels est utilise pour lacer les produceurs et visualiser les donnees de maniere interactive."
LangString DESC_TOOL_GvVoxelizer ${LANG_FRENCH} "Le voxeliseur GigaVoxels est utilise pour pre-traiter les donnees utilisateurs afin de pouvoir les visualiser avec GigaVoxels."
LangString DESC_TOOL_GvDicomVoxelizer ${LANG_FRENCH} "Le voxeliseur Dicom GigaVoxels est utilise pour pre-traiter les donnees utilisateurs Dicom afin de pouvoir les visualiser avec GigaVoxels."
; --- producers
LangString DESC_SECTION_PRODUCERS ${LANG_FRENCH} "Les producteurs GigaVoxels."
LangString DESC_PRODUCER_GvAmplifiedVolume ${LANG_FRENCH} "Lapin sous forme de champ de distance signe aggremente d'une turbulence par bruit de Perlin."
LangString DESC_PRODUCER_GvDynamicLoad ${LANG_FRENCH} "Flux de donnees I/O a partir du disque afin de charger et visualiser un modele 3D."

; Descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  
  ; Library
  ;!ifdef SECTION_LIBRARY
    !insertmacro MUI_DESCRIPTION_TEXT ${GsLibrary} $(DESC_SECTION_LIBRARY)
  ;!endif
  
  ; Tools descriptions
  !ifdef SECTION_TOOLS
    !insertmacro MUI_DESCRIPTION_TEXT ${GvTools} $(DESC_SECTION_TOOLS)
  !endif
  !ifdef TOOL_GvViewer
    !insertmacro MUI_DESCRIPTION_TEXT ${GvViewer} $(DESC_TOOL_GvViewer)
  !endif
  !ifdef TOOL_GvVoxelizer
    !insertmacro MUI_DESCRIPTION_TEXT ${GvVoxelizer} $(DESC_TOOL_GvVoxelizer)
  !endif
  !ifdef TOOL_GvDicomVoxelizer
    !insertmacro MUI_DESCRIPTION_TEXT ${GvDicomVoxelizer} $(DESC_TOOL_GvDicomVoxelizer)
  !endif
  
  ; Producer examples descriptions
  !ifdef SECTION_PRODUCERS
    !insertmacro MUI_DESCRIPTION_TEXT ${GvProducers} $(DESC_SECTION_PRODUCERS)
  !endif
  !ifdef PRODUCER_GvAmplifiedVolume
    !insertmacro MUI_DESCRIPTION_TEXT ${GvAmplifiedVolume} $(DESC_PRODUCER_GvAmplifiedVolume)
  !endif
  !ifdef PRODUCER_GvDynamicLoad
    !insertmacro MUI_DESCRIPTION_TEXT ${GvDynamicLoad} $(DESC_PRODUCER_GvDynamicLoad)
  !endif
  
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
; UNINSTALLER SECTIONS
;--------------------------------

Section "Uninstall"

  ;-- Remove shortcuts, if any
  Delete "$DESKTOP\GigaVoxels.lnk"
  ; TODO : http://nsis.sourceforge.net/Shortcuts_removal_fails_on_Windows_Vista 
  RMDir /r "$SMPROGRAMS\GigaVoxels"

  ;-- Remove directories used
  RMDir /r "$INSTDIR"

SectionEnd
