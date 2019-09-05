@rem copyright (c) 2019 okamepowder
@rem 
@setlocal
@echo off
cd "%~dp0"
IF %ERRORLEVEL% NEQ 0 goto end

IF "%VSCMD_VER%"=="" goto err_vscmd_ver

@rem Set VCTargetsPath=C:\Program Files (x86)\MSBuild\Microsoft.Cpp\v4.0\V140\

set OPTION= /p:Configuration=Release;Platform="Win32"
@rem set OPTION=%OPTION% /t:rebuild
@rem set OPTION=%OPTION% -tv:14.0

MSBuild.exe "%~dp0bsch3v_src\bsch3v\Bsch3V.sln" %OPTION%
IF %ERRORLEVEL% NEQ 0 goto end
MSBuild.exe "%~dp0ce3search_src\CE3Search.sln" %OPTION%
IF %ERRORLEVEL% NEQ 0 goto end
MSBuild.exe "%~dp0lcov_src\lcov\LCoV.sln" %OPTION%
IF %ERRORLEVEL% NEQ 0 goto end
MSBuild.exe "%~dp0nl3w_src\nl3w\nlist.sln" %OPTION%
IF %ERRORLEVEL% NEQ 0 goto end
MSBuild.exe "%~dp0nut3w_src\nu3w\nu3w.sln" %OPTION%
IF %ERRORLEVEL% NEQ 0 goto end
MSBuild.exe "%~dp0pl3w_src\pl3w\nlist.sln" %OPTION%

goto end

:err_vscmd_ver
echo ERROR: x86 Native Tools Command Prompt 

:end
pause
