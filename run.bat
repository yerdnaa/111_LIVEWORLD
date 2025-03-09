@echo off
setlocal enabledelayedexpansion



REM Вводные переменные:

REM Имя проекта:
set file_name=Varda

REM Запуск тестов:
set build_tests=ON



REM Проверяем существование директории build:
if not exist build (
    REM Если папка НЕ сущесутвует, создаём:
    mkdir build
    cd build
    mkdir resources
    echo 'build' folder created
)

if exist build (
    cd build
    echo 'build' folder exists
)


REM Удаляем старую сборку:
del %file_name%.exe
REM И удаляем старые тесты:
del %file_name%_tests.exe

REM Конфигурируем проект CMake:
cmake -G "MinGW Makefiles" -B . -S .. -D BUILD_TESTS=%build_tests% -D PROJECT_NAME_CUSTOM=%file_name%
REM                                      ^                            ^
REM                         включаем тесты                            имя проекта

REM Выполняем makefile:
mingw32-make
if exist %file_name%.exe (
    echo Build completed successfully
    REM Запускаем сборку:
    echo ====================================
    %file_name%
    if "%build_tests%"=="ON" (
        REM Запуск тестов:
        %file_name%_tests
    )
)

if not exist %file_name%.exe (
    echo Build was not completed correctly
)