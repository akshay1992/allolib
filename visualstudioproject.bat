@echo OFF
REM Sets up a visual studio project for the given target project file. 

REM TODO: Set it up for a target folder
REM TODO: Allow command line args for this script

IF [%1]==[/?] GOTO :SHOW_HELP
)

rem ------- Script environment --------
set BUILD_TYPE=Debug
set AL_APP_FILE=%1
set AL_APP_NAME=%~n1 
set AL_LIB_PATH=%cd%
rem -----------------------------------

if NOT EXIST "%AL_APP_FILE%" (
    echo == Error! No arguments passed or specified target file doesn't exist. Please specify target file to build
    EXIT /B
) else (
    echo == Building target %AL_APP_FILE%
)

pushd build 
if EXIST "%AL_APP_NAME%" (
    echo == Build directory %AL_APP_NAME% already exists. 
) else (
    mkdir %AL_APP_NAME%
)

pushd %AL_APP_NAME%
cmake -Wno-deprecated -G "Visual Studio 15 2017" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DAL_APP_FILE=..\..\%AL_APP_FILE% %AL_LIB_PATH%/cmake/single_file
IF NOT ERRORLEVEL 1 (
    echo "Visual studio solution for project is written to Project.sln in the above directory"
)
popd
popd

goto END

:SHOW_HELP
echo.
echo This script will set up a visual studio project for the given target file. 
echo Run the command with a specified target file. 
echo.
echo        %0 TARGET_FILE
echo.
echo NOTE: Currently this script doesn't support target folders.
goto END
-------------

:END