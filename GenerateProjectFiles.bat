@echo off
REM Regenerates PuzzleCaseStudy.sln and Visual Studio project files from the .uproject.
SETLOCAL

set "PROJECT_ROOT=%~dp0"
set "UPROJECT=%PROJECT_ROOT%PuzzleCaseStudy.uproject"
set "UE_ROOT=C:\Program Files\Epic Games\UE_5.1"

if not exist "%UE_ROOT%\Engine\Build\BatchFiles\GenerateProjectFiles.bat" (
    echo ERROR: Engine not found at:
    echo   %UE_ROOT%
    echo Edit UE_ROOT in GenerateProjectFiles.bat to match your Epic install.
    pause
    exit /b 1
)

call "%UE_ROOT%\Engine\Build\BatchFiles\GenerateProjectFiles.bat" "%UPROJECT%" -game -engine
pause
