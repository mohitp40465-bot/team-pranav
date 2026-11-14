@echo off
setlocal

echo ====================================================
echo  Compiling Smart Vehicle Rental Management System
echo ====================================================

:: Check if g++ is in current PATH; if not, check %USERPROFILE%\w64devkit\bin
where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    if exist "%USERPROFILE%\w64devkit\bin\g++.exe" (
        set "PATH=%USERPROFILE%\w64devkit\bin;%PATH%"
    )
)

g++ -std=c++17 -Wall -Wextra main.cpp Vehicle.cpp Customer.cpp Booking.cpp WaitingQueue.cpp RentalHistory.cpp RentalSystem.cpp -o SmartVehicleRental.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ====================================================
    echo  Build Successful! Executable: SmartVehicleRental.exe
    echo ====================================================
    echo  Run using: .\SmartVehicleRental.exe
) else (
    echo.
    echo [ERROR] Compilation failed. Please check errors above.
)
endlocal
