@echo off
::
:: Set path variables
set source=Test_TileSet_bitmask_editor.c
set output=_Test_TileSet_bitmask_editor.exe
set raylib_src=C:\raylib\raylib\src
set compF=-Wall -std=c99 -Wno-missing-braces
set libF=-lopengl32 -lgdi32 -lwinmm
::
:: Delete previous compiled file
cmd /c IF EXIST %output% del /F %output%
::
:: Compile
gcc %source% -o %output% %compF% -I %raylib_src%/ -L %raylib_src% -lraylib %libF%
::
:: Launch exe
%output%
::
pause