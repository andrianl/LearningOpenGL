@echo off
rem Open Visual Studio Code in the current directory
code .

rem Check if VS Code opened successfully
if %errorlevel% neq 0 (
    echo Failed to open Visual Studio Code.
    pause
    exit /b %errorlevel%
)
