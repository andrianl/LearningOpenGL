@echo off
rem Set the path to your premake executable
set PREMAKE_PATH="C:\Path\premake\premake5.exe"

rem Run Premake to generate Visual Studio Code project files
%PREMAKE_PATH% gmake2

rem Check if Premake succeeded
if %errorlevel% neq 0 (
    echo Premake failed to generate project files.
    pause
    exit /b %errorlevel%
)

rem Open Visual Studio Code in the current directory
code .

rem Check if VS Code opened successfully
if %errorlevel% neq 0 (
    echo Failed to open Visual Studio Code.
    pause
    exit /b %errorlevel%
)
