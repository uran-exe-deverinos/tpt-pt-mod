@echo off
REM Builds The Powder Toy with the MSVC environment loaded.
REM Usage:  build.cmd            (builds the "build" dir)
REM         build.cmd build-debug (builds another dir)
REM         build.cmd build run   (build, then launch powder.exe)
setlocal

set "DIR=%~1"
if "%DIR%"=="" set "DIR=build"

call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1

ninja -C "%~dp0%DIR%"
if errorlevel 1 exit /b 1

if /I "%~2"=="run" "%~dp0%DIR%\powder.exe"
