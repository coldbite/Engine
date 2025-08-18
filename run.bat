@echo off
echo ===================================
echo     Running Event-based Engine
echo ===================================

:: Check if build directory exists
if not exist Build (
    echo Build directory not found!
    echo Please run build.bat first.
    pause
    exit /b 1
)

:: Check if executable exists
if not exist Build\Release\Game.exe (
    if not exist Build\Debug\Game.exe (
        echo Game.exe not found!
        echo Please run build.bat first.
        pause
        exit /b 1
    ) else (
        echo Starting Debug build...
        Build\Debug\Game.exe
    )
) else (
    echo Starting Release build...
    Build\Release\Game.exe
)

echo.
echo Program finished.
pause