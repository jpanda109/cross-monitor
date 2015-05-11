msbuild ..\windows\SimulateInput\SimulateInput\SimulateInput.vcxproj /p:configuration=release /p:DebugSymbols=false /p:Debugtype=None
mkdir ..\releasesrc
mklink /H ..\releasesrc\SimulateInput.exe ..\windows\SimulateInput\SimulateInput\release\SimulateInput.exe