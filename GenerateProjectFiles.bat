@echo off

REM Шлях до виконавчого файлу Premake (змініть за необхідності)
set PREMAKE_PATH="C:\Path\premake\premake5.exe"

REM Генерація проекту для Visual Studio 2022
%PREMAKE_PATH% vs2022

REM Повідомлення про завершення генерації
echo "Visual Studio 2022 project generated successfully."

REM Пауза, щоб ви мали час переглянути повідомлення
pause
