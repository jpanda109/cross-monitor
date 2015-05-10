msbuild ..\windows\eventlisteners\windows\windows.vcxproj /p:configuration=release /p:DebugSymbols=false /p:Debugtype=None
msbuild ..\windows\MoveMouse\MoveMouse\MoveMouse.vcxproj /p:configuration=release /p:DebugSymbols=false /p:Debugtype=None
mkdir ..\releasesrc
move ..\windows\eventlisteners\windows\release\EventListener.exe ..\releasesrc
move ..\windows\MoveMouse\MoveMouse\release\MoveMouse.exe ..\releasesrc
rm ..\windows\eventlisteners\windows\release /s
move ..\windows\MoveMouse\MoveMouse\release /s