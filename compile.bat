@echo off
rem Compiles allolib in release mode
rem TODO: Add functionality to take command line options to specify debug/release
rem TODO: Add functionality to do incremental build instead of a clean rebuild everytime

rem --- Environment for this script ----
set DEBUG=1

set BUILD_DIR=build\

rem The following determines if it needs to clean BUILD_DIR before building
set INCREMENTAL_BUILD=0
rem ------------------------------------


rem Command line parser goes here
rem set DEBUG=1 or 0 depending on CLI arg. If none specified assume release

rem ------


rem Do any manipulations of the environment variables based on CLI args
if DEBUG==1 (
    echo == Building allolib in debug mode
    set BUILD_TYPE=Debug
) else (
    echo == Building allolib in release mode
    set BUILD_TYPE=Release
)

rem ----------------------------------

rem Check if we have cmake
where /q cmake.exe
IF ERRORLEVEL 1 (
    ECHO Error! Cmake is missing. Ensure it is installed and placed in your PATH.
    EXIT /B
)

rem Check if we have msbuild
where /q msbuild.exe
IF ERRORLEVEL 1 (
    ECHO Error! Msbuild is missing. Ensure it is installed and placed in your PATH.
    ECHO Better yet, run this batch file from a developer command prompt.
    EXIT /B
)

if INCREMENTAL_BUILD==0 ( if EXIST %BUILD_DIR% (
        echo == Build directory %BUILD_DIR% exists. Wiping... 
        rd /s /q %BUILD_DIR%
    ) else (
        echo == Performing incremental build (not cleaning the build dir)
    )
)

mkdir %BUILD_DIR%
pushd %BUILD_DIR%
cmake .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
msbuild allolib.sln /p:Configuration=%BUILD_TYPE%
popd