@echo off
set "build_folder=build"

if exist "%build_folder%" (
	echo found build folder. compiling project...
	cmake --build build --target clean
) else (

	 set /p version="Enter Visual Studio verison (22 or 26): "
	if "%version%"=="22" (
		cmake -G "Visual Studio 17 2022" -B build
		cmake --build build
	) else if "%version%"=="26" (
		cmake -G "Visual Studio 18 2026" -A x6x -B build
	 	cmake --build build
	) else (
		echo Invalid version. Please enter 22 or 26.
	)
)


