@echo OFF
REM Sets up a visual studio project for the given target project file. 

REM TODO: Set it up for a target folder
REM TODO: Allow command line args for this script

IF [%1]==[/?] GOTO :SHOW_HELP
)

if [%1]==[] (
    echo == Error! No arguments provided. 
    echo == For help run: 
    echo            %0 /?
    goto :END
)

rem ------- Script environment --------
set AL_APP_FILE=%1
set AL_APP_NAME=%~n1 
set AL_LIB_PATH=%cd%
rem -----------------------------------

if NOT EXIST "%AL_APP_FILE%" (
    echo == Error! Invalid arguments passed or specified target file doesn't exist. Please specify target file to build
    GOTO :SHOW_HELP
) else (
    echo == Building target %AL_APP_FILE%
)

rem Check if we have cmake
where /q cmake.exe
IF ERRORLEVEL 1 (
    ECHO Error! Cmake is missing. Ensure it is installed and placed in your PATH.
    EXIT /B
)

rem -----------------Main --------------------------
pushd build 
if EXIST "%AL_APP_NAME%" (
    echo == Build directory %AL_APP_NAME% already exists. 
) else (
    mkdir %AL_APP_NAME%
)

pushd %AL_APP_NAME%
cmake -Wno-deprecated -G "Visual Studio 15 2017" -DAL_APP_FILE=..\..\%AL_APP_FILE% %AL_LIB_PATH%/cmake/single_file
IF NOT ERRORLEVEL 1 (
    echo == Visual studio .sln for project is written to Project.sln in the above directory
)
popd
popd

goto END
rem ------------------End main ----------------------

rem -----------------help text-----------------
:SHOW_HELP
echo.
echo This script will set up a visual studio project for the given target file. 
echo Requires cmake to be installed and part of the PATH. This script is best
echo run from a "Developer Command Prompt for Visual Studio"
echo Run the command with a specified target file. 
echo.
echo        %0 TARGET_FILE
echo.
echo NOTE: Currently this script doesn't support target folders.
goto END
rem -----------------End help text-------------

:END