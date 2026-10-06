@echo off
REM Run from project root: CleanBinaries.bat
REM Closes the editor and removes game-module editor binaries so the next build is a full link.
SETLOCAL ENABLEDELAYEDEXPANSION

set "PROJECT_ROOT=%~dp0"
cd /d "%PROJECT_ROOT%"
set "BIN64=Binaries\Win64"
set "MODULE_DLL=%BIN64%\UnrealEditor-PuzzleCaseStudy.dll"

echo.
echo === PuzzleCaseStudy: clean editor binaries ===
echo     Project: %PROJECT_ROOT%
echo.

echo === Closing Unreal / Live Coding (required or DLLs stay locked)...
call :KillIfRunning UnrealEditor.exe
call :KillIfRunning UnrealEditor-Cmd.exe
call :KillIfRunning LiveCodingConsole.exe

REM Give Windows a moment to release file handles after taskkill.
timeout /t 2 /nobreak >nul

echo.
echo === Removing editor module outputs in %BIN64% ...

if not exist "%BIN64%\" (
    echo     Folder missing — nothing to clean. Run BuildEditor.bat or open the .uproject once.
    goto :AfterClean
)

call :DeleteMatching "%BIN64%\UnrealEditor-*.dll"
call :DeleteMatching "%BIN64%\UnrealEditor-*.pdb"
call :DeleteMatching "%BIN64%\UnrealEditor*.dll"
call :DeleteMatching "%BIN64%\UnrealEditor*.pdb"
call :DeleteMatching "%BIN64%\UnrealEditor-*.exp"
call :DeleteMatching "%BIN64%\UnrealEditor-*.lib"
call :DeleteMatching "%BIN64%\*.modules"
call :DeleteMatching "%BIN64%\*.target"

:AfterClean
echo.
echo === Cleaning Plugins\*\Binaries\Win64 (if any) ...

if exist "Plugins\" (
    for /d %%P in ("Plugins\*") do (
        if exist "%%P\Binaries\Win64\" (
            del /Q "%%P\Binaries\Win64\*.dll" >nul 2>&1
            del /Q "%%P\Binaries\Win64\*.pdb" >nul 2>&1
            echo     Cleaned %%P\Binaries\Win64
        )
    )
) else (
    echo     No Plugins folder (skipped).
)

echo.
if exist "%MODULE_DLL%" (
    echo WARNING: %MODULE_DLL% still exists — editor may have been running or delete failed.
    echo          Close Unreal and Rider, run this script again.
) else (
    echo OK: UnrealEditor-PuzzleCaseStudy.dll is gone — a full C++ build is required before new classes appear.
)

echo.
echo === Done ===
echo     Recommended: run RebuildEditor.bat (clean + compile), then open the editor.
echo     Opening only the .uproject may or may not show Epic's rebuild dialog (depends on launcher path).
echo.
if /I not "%~1"=="nopause" pause
ENDLOCAL
goto :eof

:DeleteMatching
set "PATTERN=%~1"
for %%F in (%PATTERN%) do (
    if exist "%%F" (
        del /Q "%%F"
        echo     Deleted %%~nxF
    )
)
goto :eof

:KillIfRunning
set "PROCNAME=%~1"
tasklist /FI "IMAGENAME eq %PROCNAME%" | find /I "%PROCNAME%" >nul
IF NOT ERRORLEVEL 1 (
    echo     %PROCNAME% is running, closing...
    taskkill /IM "%PROCNAME%" /F >nul 2>&1
) ELSE (
    echo     %PROCNAME% is not running.
)
goto :eof
