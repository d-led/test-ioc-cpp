@ECHO OFF
setlocal

color F0
set BOOST_ROOT=%~dp0\..\..\boost
set PATH=%BOOST_ROOT%\tools\jam\src\bin.ntx86;%PATH%
pushd %~dp0
cmd /k
color 0F

endlocal