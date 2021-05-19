xcopy assets "..\..\..\Debug\assets\" /s /e /y /d
xcopy assets "..\..\..\Release\assets\" /s /e /y /d

@echo off

if "%1"=="nopause" goto end
pause
:end

@echo on