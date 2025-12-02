@echo off
chcp 65001 > nul

rem ==== CAI DAT THU MUC ====
set SRC_DIR=src
set BUILD_DIR=build
set EXE_NAME=FlightSystem.exe

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo ===== KIEM TRA TRINH BIEN DICH g++ =====
where g++ >nul 2>nul
if errorlevel 1 (
    echo Khong tim thay g++. Vui long cai MinGW-w64 va them vao PATH.
    pause
    exit /b
)
echo Tim thay g++ OK.
echo.

echo ===== QUET FILE .cpp =====
setlocal enabledelayedexpansion
set CPP_FILES=

for /R "%SRC_DIR%" %%f in (*.cpp) do (
    echo Found: %%f
    set CPP_FILES=!CPP_FILES! "%%f"
)

if "!CPP_FILES!"=="" (
    echo Khong tim thay file .cpp nao trong "%SRC_DIR%".
    pause
    exit /b
)

echo.
echo ===== BAT DAU BIEN DICH =====

rem  *** CHO QUAN TRONG: -Isrc ***
g++ -std=c++17 -Wall -Wextra -O2 -Isrc !CPP_FILES! -o "%BUILD_DIR%\%EXE_NAME%"
if errorlevel 1 (
    echo.
    echo *** BIEN DICH THAT BAI ***
    pause
    exit /b
)

endlocal

echo.
echo *** BIEN DICH THANH CONG ***
echo File exe nam tai: "%BUILD_DIR%\%EXE_NAME%"
echo Hay vao thu muc build va chay file .exe thu cong.
echo.
pause
exit /b