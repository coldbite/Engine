@echo off
echo ===================================
echo    Event-based Engine Builder
echo ===================================

:: Create build directory if it doesn't exist
if not exist Build (
    echo Creating build directory...
    mkdir Build
)

:: Navigate to build directory
cd Build

:: Generate project files with CMake
echo Generating project files...
cmake -G "MinGW Makefiles" ..
if %ERRORLEVEL% neq 0 (
    echo CMake generation failed!
    exit /b %ERRORLEVEL%
)

:: Build the project
echo Building project...
cmake --build . --config Release
cd ..
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b %ERRORLEVEL%
)

echo.
echo ===================================
echo         Build Successful!
echo ===================================
echo.