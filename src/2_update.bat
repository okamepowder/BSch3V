@rem copyright (c) 2019 okamepowder
@rem 
@setlocal
@echo off
cd "%~dp0"
IF %ERRORLEVEL% NEQ 0 goto end

for /d /r %%A in (Release) do (
  for %%B in (%%A\*.exe %%A\*.dll) do (
    echo %%B
    copy "%%B" "%~dp0\..\" /B /Y
  )
)

:end
pause
