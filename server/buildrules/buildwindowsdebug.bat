msbuild ..\windows\eventlisteners\windows\windows.vcxproj /p:configuration=release /p:DebugSymbols=false /p:Debugtype=None
msbuild ..\windows\MoveMouse\MoveMouse\MoveMouse.vcxproj /p:configuration=release /p:DebugSymbols=false /p:Debugtype=None
mkdir ..\releasesrc
copy ..\windows\eventlisteners\windows\release\EventListener.exe ..\releasesrc
copy ..\windows\MoveMouse\MoveMouse\release\MoveMouse.exe ..\releasesrc