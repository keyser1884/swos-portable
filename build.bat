@echo off
setlocal enabledelayedexpansion

REM SWOS Port - Self-contained build script
REM Uses bundled MinGW and CMake

set SCRIPT_DIR=%~dp0
set MINGW_DIR=%SCRIPT_DIR%tools\mingw32
set CMAKE_EXE=%SCRIPT_DIR%tools\cmake\bin\cmake.exe
set PATH=%MINGW_DIR%\bin;%PATH%

REM Convert paths to forward slashes for CMake
set SCRIPT_DIR_FWD=%SCRIPT_DIR:\=/%
set MINGW_DIR_FWD=%MINGW_DIR:\=/%

echo ========================================
echo SWOS Port - Static Build
echo ========================================
echo.

REM Check MinGW exists
if not exist "%MINGW_DIR%\bin\gcc.exe" (
    echo ERROR: MinGW not found at %MINGW_DIR%
    echo Please ensure tools\mingw32 exists.
    pause
    exit /b 1
)

REM Check CMake exists
if not exist "%CMAKE_EXE%" (
    echo ERROR: cmake.exe not found at %CMAKE_EXE%
    pause
    exit /b 1
)

echo Using MinGW: %MINGW_DIR%
echo Using CMake: %CMAKE_EXE%
echo.

REM Clean and configure
if exist "%SCRIPT_DIR%build" rmdir /s /q "%SCRIPT_DIR%build"

echo Configuring...
"%CMAKE_EXE%" -B "%SCRIPT_DIR%build" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DSWOS_EMBED_ASSETS=ON -DCMAKE_CXX_COMPILER="%MINGW_DIR_FWD%/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="%MINGW_DIR_FWD%/bin/mingw32-make.exe"

if errorlevel 1 (
    echo.
    echo Configuration failed!
    pause
    exit /b 1
)

echo.
echo Building...
"%MINGW_DIR%\bin\mingw32-make.exe" -C "%SCRIPT_DIR%build" -j%NUMBER_OF_PROCESSORS%

if errorlevel 1 (
    echo.
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build successful!
echo ========================================
echo Output: bin\swos-port-Win32-Release.exe
echo.
pause
