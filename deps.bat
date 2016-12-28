@echo off
chcp 65001

setlocal

set curdir=%~dp0

call :getdeps "deps"
goto:eof

:getdeps
for /f "tokens=*" %%l in ('type %~1') do (
    for /f "tokens=1,2,3" %%a in ("%%l") do (
        if not defined Array[%%a] (
            if not exist "%~dp03rdparty\%%a" (
                call git clone --progress -v %%b %~dp03rdparty\%%a
            ) else (
                cd /d %curdir%3rdparty\%%a
                call git.exe pull -v --progress origin master
                cd /d %curdir%
            )

            set "Array[%%a]=true"
            echo.

            if exist "%~dp03rdparty\%%a\deps" (
                echo get deps %~dp03rdparty\%%a\deps
                call :getdeps "%~dp03rdparty\%%a\deps"
            )
        )
    )
)
exit /b 0
