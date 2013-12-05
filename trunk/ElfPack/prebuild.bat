@echo off

if %1a==a (
  echo use "$PROJ_DIR$" "$TARGET_DIR$" as arguments
  goto :eof
)

cd /d %1

call :getprojname %2

rmdir /q /s temp
mkdir temp
copy "xcl\%projectname%.xcl" temp\temp.xcl
copy "asm\%projectname%.asm" temp\temp.asm
copy "target\%projectname%.h" temp\target.h

goto :eof

:getprojname
for /f "tokens=1* delims=\/ " %%a in ( "%~1" ) do (
    set projectname=%prev%
    set prev=%%a
    call :getprojname "%%b"
)
