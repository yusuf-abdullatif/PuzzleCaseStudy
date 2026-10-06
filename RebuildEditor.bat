@echo off
REM Clean + full compile. Use after adding C++ classes (replaces "open uproject and hope for rebuild dialog").
SETLOCAL

set "PROJECT_ROOT=%~dp0"
cd /d "%PROJECT_ROOT%"

echo.
echo === RebuildEditor ===
echo.

call "%PROJECT_ROOT%CleanBinaries.bat" nopause
if errorlevel 1 exit /b 1

call "%PROJECT_ROOT%BuildEditor.bat"
exit /b %ERRORLEVEL%
