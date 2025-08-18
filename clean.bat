@echo off
echo ===================================
echo      Cleaning Build Files
echo ===================================

if exist Build (
    echo Removing build directory...
    rmdir /s /q Build
    echo Build directory cleaned successfully!
) else (
    echo No build directory found.
)

echo.
pause