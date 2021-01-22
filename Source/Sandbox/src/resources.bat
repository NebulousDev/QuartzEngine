xcopy models "..\..\..\Debug\models\" /s /e /y /d
xcopy shaders "..\..\..\Debug\shaders\" /s /e /y /d
xcopy textures "..\..\..\Debug\textures\" /s /y /e /d

xcopy models "..\..\..\Release\models\" /s /e /y /d
xcopy shaders "..\..\..\Release\shaders\" /s /e /y /d
xcopy textures "..\..\..\Release\textures\" /s /y /e /d

@echo off

if "%1"=="nopause" goto end
pause
:end

@echo on