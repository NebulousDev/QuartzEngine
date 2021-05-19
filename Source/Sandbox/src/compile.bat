%VULKAN_SDK%/Bin32/glslc.exe shader.vert -o assets/shaders/vert.spv
%VULKAN_SDK%/Bin32/glslc.exe shader.frag -o assets/shaders/frag.spv

@echo off

if "%1"=="nopause" goto:end
pause
:end

@echo on