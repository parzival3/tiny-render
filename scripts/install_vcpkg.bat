@echo on

:: Make absolute path to vcpkg directory
mkdir %~dp0..\externals\
set VCPKG_ROOTDIR=%~dp0..\externals\vcpkg

:: Ensure vcpkg repository is available (on developer machines)
if not exist "%VCPKG_ROOTDIR%\.git" (
    call git clone --depth 1 https://github.com/Microsoft/vcpkg.git %VCPKG_ROOTDIR%
)

:: Build vcpkg
call "%VCPKG_ROOTDIR%\bootstrap-vcpkg.bat" -disableMetrics
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
