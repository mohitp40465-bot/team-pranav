@echo off
echo ==========================================================
echo   Starting Smart Vehicle Rental Web Local Server (Port 5000)
echo ==========================================================
echo.
cd /d "%~dp0web"
start http://localhost:5000
python -m http.server 5000
pause
