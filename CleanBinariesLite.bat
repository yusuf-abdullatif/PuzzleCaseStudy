@echo off
REM Same as CleanBinaries.bat but does NOT close Unreal Editor.
REM Use only when the editor is already closed.
SETLOCAL

set "PROJECT_ROOT=%~dp0"
cd /d "%PROJECT_ROOT%"

echo.
echo === PuzzleCaseStudy: clean editor binaries (lite) ===
echo.

if exist "Binaries\Win64\" (
    del /Q "Binaries\Win64\UnrealEditor-*.dll" >nul 2>&1
    del /Q "Binaries\Win64\UnrealEditor-*.pdb" >nul 2>&1
    del /Q "Binaries\Win64\UnrealEditor*.dll" >nul 2>&1
    del /Q "Binaries\Win64\UnrealEditor*.pdb" >nul 2>&1
    echo     Cleaned Binaries\Win64
)

if exist "Plugins\" (
    for /d %%P in ("Plugins\*") do (
        if exist "%%P\Binaries\Win64\" (
            del /Q "%%P\Binaries\Win64\*.dll" >nul 2>&1
            del /Q "%%P\Binaries\Win64\*.pdb" >nul 2>&1
            echo     Cleaned %%P\Binaries\Win64
        )
    )
)

echo.
echo Cleanup finished.
pause
ENDLOCAL
