@echo off
echo ========================================
echo Building Test Program...
echo ========================================
echo.

REM Build the test executable
REM You may need to adjust paths based on your build configuration
REM For Debug x64:
cd x64\Debug
if exist CLI.exe (
    echo Running Automated Tests...
    echo.
    CLI.exe
) else (
    echo ERROR: CLI.exe not found!
    echo Please build the project first.
    pause
    exit /b 1
)

pause

