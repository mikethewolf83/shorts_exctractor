@echo off
REM Build script for Windows

set BUILD_DIR=build
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release

echo.
echo Build complete. Executable in %BUILD_DIR%\Release\
dir Release\shorts_extractor.exe
