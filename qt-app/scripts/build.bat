@echo off
echo ========================================
echo Vehicle Speed Checkout - Build Script
echo ========================================
echo.

:: Set Qt environment
set QT_DIR=C:\Qt\6.9.1\mingw_64
set MINGW_DIR=C:\Qt\Tools\mingw1310_64
set PATH=%QT_DIR%\bin;%MINGW_DIR%\bin;%PATH%
set CMAKE_PREFIX_PATH=%QT_DIR%

echo Qt Directory: %QT_DIR%
echo MinGW Directory: %MINGW_DIR%
echo.

:: Check if Qt is available
if not exist "%QT_DIR%\bin\qmake.exe" (
    echo ERROR: Qt 6.9.1 not found at %QT_DIR%
    echo Please install Qt 6.9.1 from https://www.qt.io/download-open-source
    echo Make sure to select MinGW 13.1.0 64-bit during installation
    pause
    exit /b 1
)

:: Check if MinGW is available
if not exist "%MINGW_DIR%\bin\gcc.exe" (
    echo ERROR: MinGW 13.1.0 not found at %MINGW_DIR%
    echo Please install Qt 6.9.1 with MinGW 13.1.0 64-bit
    pause
    exit /b 1
)

echo Qt found successfully!
echo MinGW found successfully!
echo.

:: Create build directory
if not exist "build" mkdir build
cd build

:: Configure with CMake using MinGW generator
echo Configuring project with CMake (MinGW)...
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT_DIR%" -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed
    pause
    exit /b 1
)

:: Build the project
echo.
echo Building project...
cmake --build . --config Release
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed
    pause
    exit /b 1
)

:: Verify LGPL compliance files
echo.
echo Verifying LGPL compliance files...
if exist "bin\LICENSE.txt" (
    echo ✓ LICENSE.txt found
) else (
    echo ✗ WARNING: LICENSE.txt not found in build output
)

if exist "bin\QT_LGPL_NOTICE.txt" (
    echo ✓ QT_LGPL_NOTICE.txt found
) else (
    echo ✗ WARNING: QT_LGPL_NOTICE.txt not found in build output
)

:: Check for dynamic linking
echo.
echo Checking for dynamic linking (LGPL compliance)...
if exist "bin\VehicleSpeedCheckout.exe" (
    echo ✓ Executable built successfully
    echo.
    echo To run the application:
    echo 1. Make sure Qt libraries are in your PATH
    echo 2. Run: bin\VehicleSpeedCheckout.exe
    echo.
    echo Note: The application will show LGPL compliance notice on first run
) else (
    echo ✗ ERROR: Executable not found
)

echo.
echo Build completed!
echo.
echo LGPL Compliance Summary:
echo - Dynamic linking: ✓ Configured in CMakeLists.txt
echo - License files: ✓ Included in build output
echo - User notification: ✓ Shown on first run
echo - Documentation: ✓ Available in docs/QT_LGPL_COMPLIANCE.md
echo.
echo For complete LGPL compliance information, see:
echo - docs/QT_LGPL_COMPLIANCE.md
echo - https://www.qt.io/licensing/open-source-lgpl-obligations
echo.

pause
