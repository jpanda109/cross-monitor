msbuild ..\windows\SimulateInput\SimulateInput\SimulateInput.vcxproj /p:configuration=release /p:DebugSymbols=false /p:Debugtype=None
mkdir ..\releasesrc
move ..\windows\SimulateInput\windows\release\SimulateInput.exe ..\releasesrc
rm ..\windows\SimulateInput\windows\release /s