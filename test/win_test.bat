@echo off
if not exist win_test (mkdir win_test)
set cbdebug=CB_libcfile\bin\Debug
set cbrelease=CB_libcfile\bin\Release
set vsdebug=Visual_Studio\VS17\Debug
set vsrelease=Visual_Studio\VS17\Release
set vsdebug64=Visual_Studio\VS17\Debug
set vsrelease64=Visual_Studio\VS17\Release
cd win_test
echo ----------------------------------Code::Blocks---------------------------------
echo -----Code::Blocks Debug (x86)-----
call :testblock %cbdebug% %cbdebug%
echo -----Code::Blocks Release (x86)-----
call :testblock %cbrelease% %cbrelease%
echo "----------------------------------Visual c++ (17)---------------------------------
echo ------MS Vc++ 17 Debug (x86)-----
call :testblock %vsdebug% %vsdebug%
echo -----MS Vc++ 17 Release (x86)-----
call :testblock %vsrelease% %vsrelease%
echo -----MS Vc++ 17 Debug (x64)-----
call :testblock %vsdebug64% %vsdebug64%
echo -----MS Vc++ 17 Release (x64)-----
call :testblock %vsrelease64% %vsrelease64%
echo ----------------------------------Cross-Linking---------------------------------
echo ---------------------------------------Self-------------------------------------
echo ------MS Vc++ 17 Debug (x86) DLL, MS Vc++ 17 Release (x86) EXE-----
call :testblock %vsdebug% %vsrelease%
echo ------MS Vc++ 17 Release (x86) DLL, MS Vc++ 17 Debug (x86) EXE-----
call :testblock %vsrelease% %vsdebug%
echo ------Code::Blocks Debug (x86) DLL, Code::Blocks Release (x86) EXE-----
call :testblock %cbdebug% %cbrelease%
echo ------Code::Blocks Release (x86) DLL, Code::Blocks Debug (x86) EXE-----
call :testblock %cbrelease% %cbdebug%
echo ---------------------------------------Cross-------------------------------------
echo ------MS Vc++ 17 Debug (x86) DLL, Code::Blocks Debug (x86) EXE-----
call :testblock %vsdebug% %cbdebug%
echo ------MS Vc++ 17 Debug (x86) DLL, Code::Blocks Release (x86) EXE-----
call :testblock %vsdebug% %cbrelease%
echo ------MS Vc++ 17 Release (x86) DLL, Code::Blocks Debug (x86) EXE-----
call :testblock %vsrelease% %cbdebug%
echo ------MS Vc++ 17 Release (x86) DLL, Code::Blocks Release (x86) EXE-----
call :testblock %vsrelease% %cbrelease%

echo ------Code::Blocks Debug (x86) DLL, MS Vc++ 17 Debug (x86) EXE-----
call :testblock %cbdebug% %vsdebug%
echo ------Code::Blocks Debug (x86) DLL, MS Vc++ 17 Release (x86) EXE-----
call :testblock %cbdebug% %vsrelease%
echo ------Code::Blocks Release (x86) DLL, MS Vc++ 17 Debug (x86) EXE-----
call :testblock %cbrelease% %vsdebug%
echo ------Code::Blocks Release (x86) DLL, MS Vc++ 17 Release (x86) EXE-----
call :testblock %cbrelease% %vsrelease%

if exist shared_file_test.exe (del /F /Q shared_file_test.exe)
if exist libcfile.dll (del /F /Q libcfile.dll)
cd ..
rmdir /S /Q win_test
goto :eof

:testblock
setlocal
set dllfilepath=..\..\%1\libcfile.dll
set exefilepath=..\..\%2\shared_file_test.exe
if exist shared_file_test.exe (del /F /Q shared_file_test.exe)
if exist libcfile.dll (del /F /Q libcfile.dll)
if exist %dllfilepath% (
	if exist %exefilepath% (
		@copy %dllfilepath% . >nul
		@copy %exefilepath% . >nul
		shared_file_test.exe
	)
)
endlocal

:eof
