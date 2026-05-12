@echo off
setlocal

cd /d "%~dp0"

if not defined VSCMD_VER (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64
)

if not exist build\game mkdir build\game

cl /EHsc /std:c++17 game\*.cpp common\ProtocolUtil.cpp ws2_32.lib /Fo:build\game\ /Fe:Client.exe

endlocal
