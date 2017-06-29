cd c:\Libraries\
IF NOT EXIST mpir-3.0.0 (
	echo "Downloading and building GMP / MPIR"
	appveyor DownloadFile http://mpir.org/mpir-3.0.0.zip
	7z x -y mpir-3.0.0.zip > nul
	cd mpir-3.0.0\build.vc15
	msbuild.bat gc dll x64 RELEASE > nul
	msbuild.bat gc lib x64 RELEASE > nul
	cd c:\Libraries\mpir-3.0.0\dll\x64\Release
	cp mpir.dll gmp.dll
	cp mpir.dll gmpxx.dll
	cd c:\Libraries\mpir-3.0.0\lib\x64\Release
	cp mpir.lib gmp.lib
	cp mpirxx.lib gmpxx.lib
)
set PATH=%PATH%;c:\Libraries\mpir-3.0.0\dll\x64\Release
set PATH=%PATH%;c:\Libraries\mpir-3.0.0\lib\x64\Release
echo "Looking for preinstalled Boost"
set PATH=%PATH%;C:\Libraries\boost_1_64_0
set PATH=%PATH%;C:\Libraries\boost_1_64_0\lib64-msvc-14.1
cd c:\projects\carl
md build
cd build
cmake -G "Visual Studio 14 2015 Win64" -DBUILD_STATIC="ON" -DBOOST_ROOT="%BOOST_ROOT%" -DBOOST_LIBRARYDIR="%BOOST_LIBRARYDIR%" -DBUILD_TYPE=RELEASE -DBoost_COMPILER="-vc141" ..
cmake --build . --config Release --target lib_carl
cmake --build . --config Release --target lib_carl_static
cmake --build . --config Release --target all-tests
cmake --build . --config Release --target test