@echo off
REM Builds PuzzleCaseStudyEditor (Development) without opening the Unreal Editor UI.
SETLOCAL

set "PROJECT_ROOT=%~dp0"
set "UPROJECT=%PROJECT_ROOT%PuzzleCaseStudy.uproject"
set "UE_ROOT=C:\Program Files\Epic Games\UE_5.1"

if not exist "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" (
    echo ERROR: Engine not found at:
    echo   %UE_ROOT%
    echo Edit UE_ROOT in BuildEditor.bat to match your Epic install.
    pause
    exit /b 1
)

echo Building PuzzleCaseStudyEditor Win64 Development...
call "%UE_ROOT%\Engine\Build\BatchFiles\Build.bat" PuzzleCaseStudyEditor Win64 Development "%UPROJECT%" -WaitMutex
set "ERR=%ERRORLEVEL%"

if %ERR% NEQ 0 (
    echo Build FAILED with code %ERR%.
) else (
    echo Build succeeded.
)

pause
exit /b %ERR%
